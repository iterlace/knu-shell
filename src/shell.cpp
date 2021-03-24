#include <iostream>
#include <string>
#include <iterator>

#include "shell.h"
#include "util.h"


Shell::Shell(std::istream &in, std::ostream &out, char **argv, char **envp) : istream(&in), ostream(&out) {
    for (char **var = envp; *var != nullptr; var++) {
        // push to the environment vector
        environment.emplace_back(*var);

        // parse and push to the local variables hashmap
        std::string var_s = *var;
        std::string name = var_s.substr(0, var_s.find('='));
        std::string value = var_s.substr(var_s.find('=') + 1, var_s.length() - 1);
        variables[name] = value;
    }
    for (char **arg = argv; *arg != nullptr; arg++) {
        arguments.emplace_back(*arg);
    }

    input = Input(in, out);

    commands["echo"] = &Shell::echo;
    commands["set"] = &Shell::set;
    commands["envp"] = &Shell::envp;
    commands["argc"] = &Shell::argc;
    commands["argv"] = &Shell::argv;
    commands["help"] = &Shell::help;
}

Shell::~Shell() = default;

int Shell::run() {
    while (!input.end()) {
        try {
            Command cmd = input.next();
            if (!commands.contains(cmd.name)) {
                if (!variables.contains(cmd.name)) {
                    help({});
                    continue;
                } else {
                    // print requested variable
                    echo({variables[cmd.name]});
                }
            } else {
                ShellFn method = commands[cmd.name];
                (this->*method)(cmd.args);
            }
        } catch (InvalidCommandError &e) {
            help({});
            continue;
        }
    }
    return 0;
}


std::string Shell::getVariable(std::string name) {
    if (!variables.contains(name))
        return "";
    return variables[name];
}


void Shell::echo(const CommandArgs& args) {
    std::string result;
    if (args.empty()) {
        result = "";
    } else {
        FormatTree tree(args[0], this);
        result = tree.format();
    }
    sprint(*ostream, "%s\n", result.c_str());
}

void Shell::set(const CommandArgs& args) {
    std::string key = args[0];
    std::string value = args[1];

    // store the variable
    variables[key] = value;
}


void Shell::argc(const CommandArgs& args) {
    sprint(*ostream, "%zu\n", arguments.size());
}


void Shell::argv(const CommandArgs& args) {
    for (const std::string &var : arguments)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::envp(const CommandArgs& args) {
    for (const std::string &var : environment)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::help(const CommandArgs& args) {
    sprint(*ostream, "Supported commands and instructions:\n"
                     "  variable=\"value\"\n"
                     "  echo \"<string>\"\n"
                     "  argc\n"
                     "  argv\n"
                     "  envp\n"
                     "  quit\n");
}


FormatTree::FormatTree(std::string str, Shell *shell_) : Node(shell_) {
    shell = shell_;
    source = str;
    auto iterator = str.begin();
    build(iterator, str.end());
}


bool FormatTree::build(std::string::iterator &i, std::string::iterator end) {
    // current StringNode buffer
    std::string buf;

    for (; i < end;) {
        std::unique_ptr<Node> customNode;
        if (*i == '$' && *(i + 1) == '{' && (end - i) >= 3) {
            customNode = std::make_unique<VariableNode>(shell);
        }
        // TODO: $()

        if (customNode) {
            if (customNode->build(i, end)) {
                // save the buffer
                auto previousNode = std::make_unique<StringNode>(shell);
                std::string::iterator bufIterator = buf.begin();
                previousNode->build(bufIterator, buf.end());
                buf.clear();

                children.emplace_back(std::move(previousNode));
                children.emplace_back(std::move(customNode));
                continue;
            }
        }
        // default behaviour
        buf.push_back(*i);
        i++;
    }
    if (!buf.empty()) {
        auto stringNode = std::make_unique<StringNode>(shell);
        std::string::iterator bufIterator = buf.begin();
        stringNode->build(bufIterator, buf.end());
        children.emplace_back(std::move(stringNode));
    }
    return true;
}

std::string FormatTree::format() {
    std::string buf;
    for (auto &node : children) {
        buf += node->format();
    }
    return buf;
}


bool StringNode::build(std::string::iterator &i, std::string::iterator end) {
    source = std::string(i, end);
    return true;
}

std::string StringNode::format() {
    return source;
}


const std::regex VariableNode::parseRegex = std::regex(R"(^(\$\{\s*?([a-zA-Z0-9_]*?)\s*?\}))");

bool VariableNode::build(std::string::iterator &i, std::string::iterator end) {
    std::smatch match;
    std::string s = std::string(i, end);
    if (std::regex_search(s, match, parseRegex)) {
        source = match[1];
        variableName = match[2];
        std::advance(i, source.length());
    } else {
        return false;
    }
    return true;
}

std::string VariableNode::format() {
    return shell->getVariable(variableName);
}

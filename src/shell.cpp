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


void Shell::echo(const CommandArgs &args) {
    std::string result;
    if (args.empty()) {
        result = "";
    } else {
        FormatTree tree(args[0], this);
        result = tree.getFormatted();
    }
    sprint(*ostream, "%s\n", result.c_str());
}

void Shell::set(const CommandArgs &args) {
    std::string key = args[0];
    std::string value = args[1];

    // store the variable
    variables[key] = value;
}


void Shell::argc(const CommandArgs &args) {
    sprint(*ostream, "%zu\n", arguments.size());
}


void Shell::argv(const CommandArgs &args) {
    for (const std::string &var : arguments)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::envp(const CommandArgs &args) {
    for (const std::string &var : environment)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::help(const CommandArgs &args) {
    sprint(*ostream, "Supported commands and instructions:\n"
                     "  variable=\"value\"\n"
                     "  echo \"<string>\"\n"
                     "  argc\n"
                     "  argv\n"
                     "  envp\n"
                     "  quit\n");
}


std::vector<std::unique_ptr<Node>>
Node::parseString(std::string::iterator &i, std::string::iterator end) const {
    // current StringNode buffer
    std::string buf;
    std::vector<std::unique_ptr<Node>> result;

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

                result.emplace_back(std::move(previousNode));
                result.emplace_back(std::move(customNode));
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
        result.emplace_back(std::move(stringNode));
    }
    return result;
}


std::string Node::getFormatted() {
    std::string buf;
    for (auto &node : children) {
        buf += node->getFormatted();
    }
    return buf;
}


bool StringNode::build(std::string::iterator &i, std::string::iterator end) {
    source = std::string(i, end);
    return true;
}

std::string StringNode::getFormatted() {
    return source;
}


const std::regex VariableNode::parseRegex = std::regex(R"(^(\$\{\s*?([a-zA-Z0-9_]*?)\s*?\}))");

bool VariableNode::build(std::string::iterator &i, std::string::iterator end) {
    std::smatch match;
    std::string str = std::string(i, end);
    if (std::regex_search(str, match, parseRegex)) {
        // variable data
        source = match[1];
        std::string name = match[2];
        std::string value = shell->getVariable(name);

        // increment iterator
        std::advance(i, source.length());

        auto valueIterator = value.begin();
        auto nodes = parseString(valueIterator, value.end());
        if (!nodes.empty()) {
            for (auto &node : nodes)
                children.emplace_back(std::move(node));
        } else {
            auto valueNode = std::make_unique<StringNode>(shell);
            valueIterator = value.begin();
            valueNode->build(valueIterator, value.end());
            children.emplace_back(std::move(valueNode));
        }
    } else {
        return false;
    }
    return true;
}


FormatTree::FormatTree(std::string str, Shell *shell_) : Node(shell_) {
    shell = shell_;
    source = str;
    auto iterator = str.begin();
    build(iterator, str.end());
}


bool FormatTree::build(std::string::iterator &i, std::string::iterator end) {
    auto nodes = parseString(i, end);
    for (auto &node : nodes)
        children.emplace_back(std::move(node));
    return true;
}

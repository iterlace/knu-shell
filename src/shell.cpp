#include <iostream>
#include <string>

#include "shell.h"
#include "util.h"
#include "string_parser.h"


Shell::Shell(std::istream& in, std::ostream& out, char **argv, char **envp) : istream(&in), ostream(&out) {
    for (char **var = envp; *var != nullptr; var++) {
        // push to the environment vector
        environment.emplace_back(*var);

        // parse and push to the local variables hashmap
        std::string var_s = *var;
        std::string name = var_s.substr(0, var_s.find('='));
        std::string value = var_s.substr(var_s.find('=')+1, var_s.length()-1);
        variables[name] = value;
    }
    for (char **arg = argv; *arg != nullptr; arg++) {
        arguments.emplace_back(*arg);
    }

    input = Input(in, out);

    commands["echo"] = &Shell::echo;
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
            std::optional<ShellFn> fn = find_executor(cmd);
            if (!fn.has_value()) {
                throw InvalidCommandError();
            } else {
                (this->*fn.value())(cmd);
            }
        } catch (InvalidCommandError& e) {
            help();
            continue;
        }
    }
    return 0;
}


std::optional<Shell::ShellFn> Shell::find_executor(const Command& command) {
    const auto& tokens = command.get_tokens();

    if (auto v = dynamic_cast<VariableToken *>(tokens[0])) {
        return &Shell::set;
    } else if (auto c = dynamic_cast<CommandToken *>(tokens[0])) {
        std::string cmd_name = c->to_str();
        if (commands.contains(cmd_name)) {
            return commands[cmd_name];
        }
    }
    return std::nullopt;
}

std::string Shell::format_string(const StringToken& string_token) const {
    std::string final_string;
    for (auto token : string_token.get_vector()) {
        if (auto text = dynamic_cast<TextToken *>(token)) {
            final_string += token->to_str();
        } else if (auto link = dynamic_cast<LinkToken *>(token)) {
            std::string variable_name = link->to_str();
            if (variables.contains(variable_name)) {
                auto parser = StringParser(variables.find(variable_name)->second);
                StringToken escaped = parser.run();
                final_string += format_string(escaped);
            }
        }
    }
    return final_string;
}



void Shell::echo(const Command& command) {
    auto& tokens = command.get_tokens();
    if (tokens.size() == 1) {
        sprint(*ostream, "\n");
        return;
    }
    if (auto text = dynamic_cast<StringToken *>(tokens[1])) {
        std::string formatted_text = format_string(*text);
        sprint(*ostream, "%s\n", formatted_text.c_str());
    }
}

void Shell::set(const Command& command) {
    auto& tokens = command.get_tokens();
    auto name = dynamic_cast<VariableToken *>(tokens[0]);
    auto value = dynamic_cast<StringToken *>(tokens[2]);
    if (!name || !value) {
        throw InvalidCommandError();
    }

    variables[name->to_str()] = value->to_str();
}


void Shell::argc(const Command& command) {
    sprint(*ostream, "%zu\n", arguments.size());
}


void Shell::argv(const Command& command) {
    for (const std::string& var : arguments)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::envp(const Command& command) {
    for (const std::string& var : environment)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::help() {
    sprint(*ostream, "Supported commands and instructions:\n"
                     "  variable=\"value\"\n"
                     "  echo \"string\"\n"
                     "  argc\n"
                     "  argv\n"
                     "  envp\n"
                     "  quit\n");
}

void Shell::help(const Command& command) {
    help();
}




#include <iostream>
#include <fstream>
#include <string>

#include "shell.h"
#include "util.h"
#include "string_parser.h"


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
}

Shell::~Shell() = default;

int Shell::run() {
    while (!input.end()) {
        try {
            Command cmd = input.next();
            history.push_back(cmd.to_str());
            std::optional<ShellFn> fn = find_executor(cmd);
            if (!fn.has_value()) {
                throw InvalidCommandError();
            } else {
                (this->*fn.value())(cmd);
            }
        } catch (InvalidCommandError &e) {
            help();
            continue;
        }
    }
    return 0;
}


std::optional<Shell::ShellFn> Shell::find_executor(const Command &command) {
    const auto &tokens = command.get_tokens();

    if (tokens[0].get()->type() == VARIABLE) {
        return &Shell::set;
    } else if (auto c = dynamic_cast<CommandToken *>(tokens[0].get())) {
        std::string cmd_name = c->to_str();
        if (commands.contains(cmd_name)) {
            return commands[cmd_name];
        }
    }
    return std::nullopt;
}

std::string Shell::format_string(const StringToken &string_token) const {
    std::string final_string;
    for (auto &token : string_token.get_vector()) {
        if (auto text = dynamic_cast<TextToken *>(token.get())) {
            final_string += token->to_str();
        } else if (auto link = dynamic_cast<LinkToken *>(token.get())) {
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


void Shell::echo(const Command &command) {
    auto &tokens = command.get_tokens();
    if (tokens.size() == 1) {
        sprint(*ostream, "\n");
        return;
    }
    if (auto text = dynamic_cast<StringToken *>(tokens[1].get())) {
        std::string formatted_text = format_string(*text);
        sprint(*ostream, "%s\n", formatted_text.c_str());
    }
}

void Shell::set(const Command &command) {
    auto &tokens = command.get_tokens();
    auto name = dynamic_cast<VariableToken *>(tokens[0].get());
    auto value = dynamic_cast<StringToken *>(tokens[2].get());
    if (!name || !value) {
        throw InvalidCommandError();
    }

    variables[name->to_str()] = value->to_str();
}


void Shell::argc(const Command &command) {
    sprint(*ostream, "%zu\n", arguments.size());
}


void Shell::argv(const Command &command) {
    for (const std::string &var : arguments)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::envp(const Command &command) {
    for (const std::string &var : environment)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::save(const Command &command) {
    auto &tokens = command.get_tokens();
    auto filepath_token = dynamic_cast<StringToken *>(tokens[1].get());
    std::string filepath = format_string(*filepath_token);

    std::ofstream file(filepath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        sprint(*ostream, "Error writing to the file %s\n", filepath.c_str());
        return;
    }
    for (const auto &record : history) {
        file << record << std::endl;
    }
    file.close();

    sprint(*ostream, "History saved to %s\n", filepath.c_str());
}


void Shell::load(const Command &command) {
    auto &tokens = command.get_tokens();
    auto filepath_token = dynamic_cast<StringToken *>(tokens[1].get());
    std::string filepath = format_string(*filepath_token);

    std::ifstream file(filepath, std::ios::in);
    if (!file.is_open()) {
        sprint(*ostream, "Error opening file %s\n", filepath.c_str());
        return;
    }

    std::string record;
    history.clear();
    while (std::getline(file, record)) {
        history.push_back(record);
        sprint(*ostream, "%s\n", record.c_str());
    }
    file.close();
}


void Shell::help() {
    sprint(*ostream, "Supported commands and instructions:\n"
                     "  variable=\"value\"\n"
                     "  echo \"string\"\n"
                     "  argc\n"
                     "  argv\n"
                     "  envp\n"
                     "  save \"/file/path.txt\"\n"
                     "  load \"/file/path.txt\"\n"
                     "  quit\n");
}

void Shell::help(const Command &command) {
    help();
}




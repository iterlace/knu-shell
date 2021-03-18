#include <iostream>
#include <string>

#include "shell.h"
#include "util.h"


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
        } catch (InvalidCommandError& e) {
            help({});
            continue;
        }
    }
    return 0;
}


void Shell::echo(CommandArgs args) {
    std::string content = args[0];
    char buf[512] = {0};

    // i - current 'content' position. skips double quotes at start and in the end
    // buf_i - current 'buf' position
    for (int i=0, buf_i=0; i < content.length();) {
        // process a variable placeholder
        if (content[i] == '$' && content[i + 1] == '{') {
            // find a related closing bracket
            int closing_bracket = content.find('}', i);
            if (closing_bracket == -1) {
                sprint(*ostream, "Syntax error: variable placeholder \"${\" at position %d is not enclosed with \"}\". "
                        "If \"${\" are not special symbols, then use a preceding backslash "
                        "(e.g. \"\\${\" or \"$\\{\").\n", i);
                return;
            }

            // a variable key, enclosed in ${ }
            std::string key = content.substr(i + 2, closing_bracket - (i + 2));
            if (!variables.contains(key)) {
                sprint(*ostream, "Variable \"%s\" is not set!\n", key.c_str());
                return;
            }
            std::string value = variables[key];

            // write variable to the result string
            for (char c : value)
                buf[buf_i++] = c;

            i = closing_bracket+1;
            continue;
        } else {
            // if ran onto escape char (\) - skip it and print the next character in current iteration
            if (content[i] == '\\')
                i++;
            // copy the character
            buf[buf_i++] = content[i++];
        }
    }
    sprint(*ostream, "%s\n", buf);
}

void Shell::set(CommandArgs args) {
    std::string key = args[0];
    std::string value = args[1];

    // store the variable
    variables[key] = value;
}


void Shell::argc(CommandArgs args) {
    sprint(*ostream, "%zu\n", arguments.size());
}


void Shell::argv(CommandArgs args) {
    for (const std::string& var : arguments)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::envp(CommandArgs args) {
    for (const std::string& var : environment)
        sprint(*ostream, "%s\n", var.c_str());
}


void Shell::help(CommandArgs args) {
    sprint(*ostream, "Supported commands and instructions:\n"
           "  variable=\"value\"\n"
           "  echo \"<string>\"\n"
           "  argc\n"
           "  argv\n"
           "  envp\n"
           "  quit\n");
}



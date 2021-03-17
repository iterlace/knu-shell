#include <iostream>
#include <string>

#include "shell.h"


Shell::Shell(char **argv, char **envp) {
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
}

Shell::~Shell() = default;

int Shell::run() {
    std::string input;
    while (true) {
        printf("prompt> ");
        getline(std::cin, input);

        if (input.starts_with("echo ")) {
            echo(input.substr(5));
        }
        else if (input.find('=') != -1) {
            set(input);
        }
        else if (input == "argc") {
            argc();
        }
        else if (input == "argv") {
            argv();
        }
        else if (input == "envp") {
            envp();
        }
        else if (input == "help") {
            help();
        }
        else if (input == "exit") {
            printf("bye-bye\n");
            break;
        }
        else {
            help();
        }
    }
    return 0;
}


void Shell::echo(const std::string& args) {
    // check if content is enclosed in quotes
    if (args[0] != '"' || args[args.length()-1] != '"') {
        printf("Usage: echo \"Text content\"\n");
        return;
    }
    char buf[512] = {0};

    // i - current 'args' position. skips double quotes at start and in the end
    // buf_i - current 'buf' position
    for (int i=1, buf_i=0; i < args.length()-1;) {
        // process a variable placeholder
        if (args[i] == '$' && args[i + 1] == '{') {
            // find a related closing bracket
            int closing_bracket = args.find('}', i);
            if (closing_bracket == -1) {
                printf("Syntax error: variable placeholder \"${\" at position %d is not enclosed with \"}\". "
                       "If \"${\" are not special symbols, then use a preceding backslash "
                       "(e.g. \"\\${\" or \"$\\{\").\n", i);
                return;
            }

            // a variable key, enclosed in ${ }
            std::string key = args.substr(i + 2, closing_bracket - (i + 2));
            if (!variables.contains(key)) {
                printf("Variable \"%s\" is not set!\n", key.c_str());
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
            if (args[i] == '\\')
                i++;
            // copy the character
            buf[buf_i++] = args[i++];
        }
    }
    printf("%s\n", buf);
}

void Shell::set(const std::string& args) {
    int equality_pos = args.find('=');
    std::string key = args.substr(0, equality_pos);
    std::string value = args.substr(equality_pos+1);

    // check if variable is enclosed in quotes
    if (value.length() < 2 || value[0] != '"' || value[value.length()-1] != '"') {
        printf("Usage: key=\"value\"\n");
        return;
    }
    // skip double quotes
    value = value.substr(1, value.length()-2);

    // store the variable
    variables[key] = value;
}


void Shell::argc() {
    printf("%zu\n", arguments.size());
}


void Shell::argv() {
    for (const std::string& var : arguments)
        printf("%s\n", var.c_str());
}


void Shell::envp() {
    for (const std::string& var : environment)
        printf("%s\n", var.c_str());
}


void Shell::help() {
    printf("Supported commands and instructions:\n"
           "  variable=\"value\"\n"
           "  echo \"<string>\"\n"
           "  argc\n"
           "  argv\n"
           "  envp\n"
           "  exit\n");
}



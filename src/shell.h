#include <string>
#include <vector>
#include <map>

#include "input.h"

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

class Shell {
public:
    Shell(char *argv[], char *envp[]);
    ~Shell();

    int run();
private:
    // program arguments
    std::vector<std::string> arguments;
    // environment variables
    std::vector<std::string> environment;
    // environment vars and added at runtime variables
    std::map<std::string, std::string> variables;
    // commands
    std::map<CommandName, void(Command)> commands;

    // Commands
    void echo(const std::string& args);
    void set(const std::string& args);
    void argc();
    void argv();
    void envp();
    void help();
};

#endif //SHELL_SHELL_H





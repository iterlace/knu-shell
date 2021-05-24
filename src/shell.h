#include <string>
#include <vector>
#include <map>

#include <functional>
#include <iostream>
#include <string>
#include <iomanip>
#include <deque>
#include <map>
#include <cassert>
#include <optional>

#include "input.h"

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H



struct NodeParsingError : public std::exception {};


class Shell {
public:
    Shell(std::istream &in, std::ostream &out, char *argv[], char *envp[]);
    ~Shell();

    int run();
private:
    std::istream *istream;
    std::ostream *ostream;
    Input input;

    typedef void (Shell::*ShellFn)(const Command& command);

    // program arguments
    std::vector<std::string> arguments;
    // environment variables
    std::vector<std::string> environment;
    // environment vars and added at runtime variables
    std::map<std::string, std::string> variables;
    //
    std::vector<std::string> history;

    std::optional<ShellFn> find_executor(const Command& command);
    std::string format_string(const StringToken& string_token) const;

    // Commands
    void echo(const Command& command);
    void set(const Command& command);
    void argc(const Command& command);
    void argv(const Command& command);
    void envp(const Command& command);
    void save(const Command& command);
    void load(const Command& command);
    void help();
    void help(const Command& command);

    // commands
    std::map<std::string, ShellFn> commands = {
        {"echo", &Shell::echo},
        {"envp", &Shell::envp},
        {"argc", &Shell::argc},
        {"argv", &Shell::argv},
        {"save", &Shell::save},
        {"load", &Shell::load},
        {"help", &Shell::help},
    };
};




#endif //SHELL_SHELL_H





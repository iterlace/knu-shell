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

    typedef void (Shell::*ShellFn)(CommandArgs args);
    // program arguments
    std::vector<std::string> arguments;
    // environment variables
    std::vector<std::string> environment;
    // environment vars and added at runtime variables
    std::map<std::string, std::string> variables;
    // commands
    std::map<CommandName, ShellFn> commands;


    // Commands
    void echo(CommandArgs args);
    void set(CommandArgs args);
    void argc(CommandArgs args);
    void argv(CommandArgs args);
    void envp(CommandArgs args);
    void help(CommandArgs args);
};



enum NodeType {
    STRING,
    VARIABLE,
};

class Node {
public:
    // original string
    std::string source;
    std::vector<Node*> children;

    explicit Node(Shell *shell_) : shell(shell_) {};
    virtual ~Node();

    virtual char* parse(char* start, char* end);
    virtual std::string build();
protected:
    Shell *shell;
};


class StringNode : public Node {
public:
    static const NodeType type = NodeType::STRING;

    explicit StringNode(Shell *shell_) : Node(shell_) {};

    char* parse(char* start, char* end);
    std::string build();
};


class VariableNode : public Node {
public:
    static const NodeType type = NodeType::VARIABLE;

    explicit VariableNode(Shell *shell_) : Node(shell_) {};

    char* parse(char* start, char* end);
    std::string build();
private:

};



class FormatTree : public Node {
public:
    FormatTree(std::string const &str, Shell *shell_);

    char* parse(char* start, char* end);
    std::string build();
};




#endif //SHELL_SHELL_H





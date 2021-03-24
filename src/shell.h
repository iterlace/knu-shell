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

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include "input.h"


struct NodeParsingError : public std::exception {
};


class Shell {
public:
    Shell(std::istream &in, std::ostream &out, char *argv[], char *envp[]);

    ~Shell();

    /**
     * Runs the shell interpreter. Exits on "quit" command.
     */
    int run();

    /**
     * Returns a saved variable
     * @param name variable name
     * @return saved value or empty string if nothing found
     */
    std::string getVariable(std::string name);

private:
    // input stream
    std::istream *istream;
    // output stream
    std::ostream *ostream;
    // related Input object
    Input input;

    // commands handlers
    typedef void (Shell::*ShellFn)(const CommandArgs &);

    // program arguments
    std::vector<std::string> arguments;
    // environment variables
    std::vector<std::string> environment;
    // environment vars and added at runtime variables
    std::map<std::string, std::string> variables;
    // commands
    std::map<CommandName, ShellFn> commands;


    // Commands
    void echo(const CommandArgs &args);

    void set(const CommandArgs &args);

    void argc(const CommandArgs &args);

    void argv(const CommandArgs &args);

    void envp(const CommandArgs &args);

    void help(const CommandArgs &args);
};


/**
 * Format tree classes
 */

enum NodeType {
    STRING,
    VARIABLE,
    // COMMAND,
};

class Node {
public:
    explicit Node(Shell *shell_) : shell(shell_) {};

    /**
     * Builds current component from the given string
     * @param i, @param end - string bounds to process
     * @return success
     */
    virtual bool build(std::string::iterator &i, std::string::iterator end) = 0;

    /**
     * @return formatted string
     */
    virtual std::string getFormatted();

protected:
    // original string
    std::string source;
    // children nodes
    std::vector<std::unique_ptr<Node>> children;
    // related shell
    Shell *shell;

    std::vector<std::unique_ptr<Node>> parseString(std::string::iterator &i, std::string::iterator end) const;
};


class StringNode : public Node {
public:
    static const NodeType type = NodeType::STRING;

    explicit StringNode(Shell *shell_) : Node(shell_) {};

    bool build(std::string::iterator &i, std::string::iterator end) override;

    std::string getFormatted() override;
};


class VariableNode : public Node {
public:
    static const NodeType type = NodeType::VARIABLE;

    explicit VariableNode(Shell *shell_) : Node(shell_) {};

    bool build(std::string::iterator &i, std::string::iterator end) override;

private:
    static const std::regex parseRegex;
};


class FormatTree : public Node {
public:
    FormatTree(std::string str, Shell *shell_);

    bool build(std::string::iterator &i, std::string::iterator end) override;
};


#endif //SHELL_SHELL_H





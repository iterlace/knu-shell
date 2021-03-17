
#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <utility>
#include <string>
#include <regex>
#include <queue>


typedef std::string CommandName;
typedef std::vector<std::string> CommandArgs;

struct Command {
    CommandName name;
    CommandArgs args;

    Command(CommandName _name, CommandArgs _args) : name(_name), args(_args) {};
};

struct InvalidCommandError : public std::exception {};


class Input {
public:
    Input();
    ~Input();

    /**
     * Reads user input and returns a parsed command
     */
    Command next();

    /**
     * @return is session closed
     */
    bool end();
private:
    std::queue<std::string> inputBuffer;

    /**
     * Reads latest user input
     * @return
     */
    std::string read();



    // Input processing methods


    /**
     * Parse an entire command
     * @param input the entire string from user
     * @return
     * @throws InvalidCommandError
     */
    static Command parse(std::string const &input);

    /**
     *
     * @param input
     * @return
     */
    static std::string strip(std::string const &input);

    /**
     * Splits arguments string by spaces
     * @param input arguments string
     * @return vector of parsed arguments
     */
    static CommandArgs parseArgs(std::string const &input);



    // String processing regex patterns


    /**
     * Example:
     *   "  Hello, world  "  => "Hello, world"
     * Groups:
     *   1: stripped content
     */
    static const std::regex stripRegex;

    /**
     * Example:
     *   "--log=DEBUG -o \"/var/log/test.txt\"" => {"--log=DEBUG", "-o", "/var/log/test.txt"}
     * Groups:
     *   1: entire argument body
     */
    static const std::regex argumentsRegex;

    /**
     * Example:
     *   "echo \"Hello, World\"" => {"echo", "\"Hello, World\""}
     * Groups:
     *   1: command name (e.g. "echo")
     *   2: arguments
     */
    static const std::regex commandRegex;

    /**
     * Example:
     *   "\\A" => "A"
     * Groups:
     *   1: a character BEFORE the deleted backslash.
     *
     * When using in regex_replace, you MUST pass "$1" formatting rather than ""
     */
    static const std::regex reduceEscapeCharRegex;

    /**
     * Example:
     *   "A=123" => {"A", "123"}
     * Groups:
     *   1: variable name
     *   2: value
     */
    static const std::regex assignmentRegex;
};



#endif //SHELL_INPUT_H

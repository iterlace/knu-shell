
#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <utility>
#include <string>
#include <regex>
#include <queue>
#include <vector>
#include <string>

#include "fsm.h"


class Token {
public:
    virtual std::string to_str() const = 0;
};

/**
 * Base class for all flat, string-based tokens
 */
class BaseStringToken : public Token {
public:
    BaseStringToken() = default;

    BaseStringToken(std::string s);

    void push_back(char c);

    std::string to_str() const override;
    std::string get_raw() const;
protected:
    std::string value;
};


class TempToken : public BaseStringToken {
public:
};


// Command name (e.g "echo", "argv")
class CommandName : public BaseStringToken {
public:
    CommandName(std::string s) : BaseStringToken(std::move(s)) {};
    CommandName(const TempToken &token);
};


// Resolved variable name (e.g USER="John", where USER is a VariableName)
class VariableName : public BaseStringToken {
public:
    VariableName(const TempToken &token);
};


class AssignmentToken : public Token {
public:
    AssignmentToken() = default;
    std::string to_str() const override;
};


// Pure text without links
class Text : public BaseStringToken {
public:
};


// Link to some variable
class Link : public BaseStringToken {
public:
};


/**
 * String is a composite class, containing Text and Link
 */
class String : public Token {
public:
    String();

    ~String();

    template<class T>
    void push_back(T token) {
        static_assert(std::is_base_of<Token, T>::value, "Must be derived from Token");
        T *t = new T;
        *t = token;
        literals.push_back(t);
    };

    const std::vector<Token *>& get_vector();

    std::string format(std::map<std::string, std::string> variables);
    std::string to_str() const override;

protected:
    std::vector<Token *> literals;
};


class Command {
public:
    Command(const std::vector<Token *>&);
    ~Command();

private:
    std::vector<Token *> tokens;
};


struct InvalidCommandError : public std::exception {
};


class Input {
public:
    Input();

    Input(std::istream &in, std::ostream &out);

    ~Input();

    /**
     * Reads user input and returns a parsed command
     */
    Command next();

    /**
     * @return is session closed
     */
    bool end();

    /**
     * Parse an entire command
     * @param input the entire string from user
     * @return Command
     * @throws InvalidCommandError
     */
    static Command parse(std::string const &input);

private:
    std::istream *istream;
    std::ostream *ostream;
    std::queue<std::string> inputBuffer;

    /**
     * Reads latest user input
     * @return
     */
    std::string read();
};


#endif //SHELL_INPUT_H

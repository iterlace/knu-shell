
#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <iostream>
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
class CommandToken : public BaseStringToken {
public:
    CommandToken(std::string s) : BaseStringToken(std::move(s)) {};
    CommandToken(const TempToken &token);
};


// Resolved variable name (e.g USER="John", where USER is a VariableToken)
class VariableToken : public BaseStringToken {
public:
    VariableToken(const TempToken &token);
};


class AssignmentToken : public Token {
public:
    AssignmentToken() = default;
    std::string to_str() const override;
};


// Pure text without links
class TextToken : public BaseStringToken {
public:
};


// Link to some variable
class LinkToken : public BaseStringToken {
public:
};


/**
 * StringToken is a composite class, containing TextToken and LinkToken
 */
class StringToken : public Token {
public:
    StringToken() = default;
    StringToken(const std::vector<Token *>&);
    ~StringToken();

    template<class T>
    void push_back(T token) {
        static_assert(std::is_base_of<Token, T>::value, "Must be derived from Token");
        T *t = new T;
        *t = token;
        literals.push_back(t);
    };

    const std::vector<Token *>& get_vector() const;

    std::string to_str() const override;

protected:
    std::vector<Token *> literals;
};


class Command {
public:
    Command(const std::vector<Token *>&);
    ~Command();

    const std::vector<Token*>& get_tokens() const;
private:
    std::vector<Token *> tokens;
};


struct InvalidCommandError : public std::exception {
};


class Input {
public:
    Input() : istream(&std::cin), ostream(&std::cout) {};
    Input(std::istream &in, std::ostream &out) : istream(&in), ostream(&out) {};
    ~Input() = default;

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
    std::queue<std::string> input_buffer;

    /**
     * Reads latest user input
     * @return
     */
    std::string read();
};


#endif //SHELL_INPUT_H

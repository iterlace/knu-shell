
#ifndef SHELL_TOKENS_H
#define SHELL_TOKENS_H

#include <string>
#include <memory>
#include <vector>

typedef unsigned char TokenType;
enum TokenTypes : TokenType {
    TEMP,
    COMMAND,
    VARIABLE,
    ASSIGNMENT,
    TEXT,
    LINK,
    STRING,
};


class Token {
public:
    virtual std::string to_str() const = 0;
    virtual bool operator==(Token &other) = 0;
    virtual bool operator!=(Token &other) = 0;

    virtual TokenTypes type() const = 0;
};

/**
 * Base class for all flat, string-based tokens
 */
class BaseStringToken : public Token {
public:
    BaseStringToken() = default;
    BaseStringToken(std::string s);

    bool operator==(Token &other) override;
    bool operator!=(Token &other) override;

    void push_back(char c);

    std::string to_str() const override;
    std::string get_raw() const;
protected:
    std::string value;
};


class TempToken : public BaseStringToken {
public:
    TempToken() : BaseStringToken() {};
    TempToken(std::string s) : BaseStringToken(move(s)) {};

    TokenTypes type() const override { return TEMP; };
};


// Command name (e.g "echo", "argv")
class CommandToken : public BaseStringToken {
public:
    CommandToken() = default;
    CommandToken(std::string s) : BaseStringToken(std::move(s)) {};
    CommandToken(const TempToken &token);


    TokenTypes type() const override { return COMMAND; };
};


// Resolved variable name (e.g USER="John", where USER is a VariableToken)
class VariableToken : public BaseStringToken {
public:
    VariableToken(const std::string &s);
    VariableToken(const TempToken &token);

    TokenTypes type() const override { return VARIABLE; };
};


class AssignmentToken : public Token {
public:
    AssignmentToken() = default;

    bool operator==(Token &other) override;
    bool operator!=(Token &other) override;

    TokenTypes type() const override { return ASSIGNMENT; };

    std::string to_str() const override;
};


// Pure text without links
class TextToken : public BaseStringToken {
public:
    TextToken() : BaseStringToken() {};
    TextToken(std::string s) : BaseStringToken(move(s)) {};

    TokenTypes type() const override { return TEXT; };
};


// Link to some variable
class LinkToken : public BaseStringToken {
public:
    LinkToken() : BaseStringToken() {};
    LinkToken(std::string s) : BaseStringToken(move(s)) {};

    TokenTypes type() const override { return LINK; };
};


/**
 * StringToken is a composite class, containing TextToken and LinkToken
 */
class StringToken : public Token {
public:
    StringToken() = default;
    StringToken(const std::vector<std::shared_ptr<Token>>&);

    TokenTypes type() const override { return STRING; };

    bool operator==(Token &other) override;
    bool operator!=(Token &other) override;

    template<class T>
    void push_back(T token) {
        static_assert(std::is_base_of<Token, T>::value, "Must be derived from Token");
        auto t = std::make_shared<T>();
        *t = token;
        literals.push_back(t);
    };

    const std::vector<std::shared_ptr<Token>>& get_vector() const;

    std::string to_str() const override;

protected:
    std::vector<std::shared_ptr<Token>> literals;
};


#endif //SHELL_TOKENS_H

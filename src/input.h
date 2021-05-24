
#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <iostream>
#include <utility>
#include <string>
#include <regex>
#include <queue>
#include <vector>
#include <string>
#include <memory>

#include "fsm.h"
#include "tokens.h"



class Command {
public:
    Command(const std::vector<std::shared_ptr<Token>>&, std::string origin);
    bool operator==(const Command &other);
    bool operator!=(const Command &other);

    const std::vector<std::shared_ptr<Token>>& get_tokens() const;
    std::string to_str() const;
private:
    std::vector<std::shared_ptr<Token>> tokens;
    std::string origin;
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

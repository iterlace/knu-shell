#include <iostream>

#include "input.h"

#include "input_parser.h"
#include "util.h"



Command::Command(const std::vector<std::shared_ptr<Token>> &tokens, std::string origin) {
    this->tokens.assign(tokens.begin(), tokens.end());
    this->origin = origin;
}

bool Command::operator==(const Command &other) {
    if (tokens.size() != other.get_tokens().size())
        return false;
    for (int i=0; i < tokens.size(); i++)
        if (*tokens[i].get() != *other.get_tokens()[i].get())
            return false;
    return true;
}

bool Command::operator!=(const Command &other) {
    return !(this->operator==(other));
}

const std::vector<std::shared_ptr<Token>> &Command::get_tokens() const {
    return tokens;
}

std::string Command::to_str() const {
    return origin;
}


Command Input::next() {
    if (input_buffer.empty()) {
        input_buffer.push(read());
    }
    std::string input = input_buffer.front();
    input_buffer.pop();
    Command cmd = parse(input);
    return cmd;
}


bool Input::end() {
    if (input_buffer.empty()) {
        input_buffer.push(read());
    }
    return input_buffer.front() == "quit";
}

std::string Input::read() {
    std::string input;
    sprint(*ostream, "prompt> ");
    getline(*istream, input);
    return input;
}


Command Input::parse(std::string const &input) {
    InputParser parser{input};
    try {
        Command c = parser.run();
        return c;
    } catch (InvalidInput &e) {
        throw InvalidCommandError();
    }
}
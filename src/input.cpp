#include <iostream>

#include "input.h"

#include "input_parser.h"
#include "util.h"


BaseStringToken::BaseStringToken(std::string s) {
    value.assign(s.begin(), s.end());
}

void BaseStringToken::push_back(char c) {
    value.push_back(c);
}

std::string BaseStringToken::to_str() const {
    return value;
}

std::string BaseStringToken::get_raw() const {
    return value;
}

CommandToken::CommandToken(const TempToken &token) {
    value = token.get_raw();
}

VariableToken::VariableToken(const TempToken &token) {
    value = token.get_raw();
}

std::string AssignmentToken::to_str() const {
    return "=";
}

StringToken::StringToken(const std::vector<Token *> &tokens) {
    for (auto token : tokens) {
        if (auto text = dynamic_cast<TextToken *>(token)) {
            auto owned_text = new TextToken();
            *owned_text = *text;
            literals.push_back(owned_text);
        } else if (auto link = dynamic_cast<LinkToken *>(token)) {
            auto owned_link = new LinkToken();
            *owned_link = *link;
            literals.push_back(owned_link);
        } else {
            throw;
        }
    }
}

StringToken::~StringToken() {
    for (int i = 0; i < literals.size(); i++) {
        delete literals[i];
    }
}

const std::vector<Token *> &StringToken::get_vector() const {
    return literals;
}


std::string StringToken::to_str() const {
    std::string output;
    for (auto token : literals) {
        if (auto text = dynamic_cast<TextToken *>(token)) {
            output += text->to_str();
        } else if (auto link = dynamic_cast<LinkToken *>(token)) {
            output += "${" + link->to_str() + "}";
        }
    }
    return output;
}


Command::Command(const std::vector<Token *> &tokens) {
    this->tokens.assign(tokens.begin(), tokens.end());
}

Command::~Command() {
    for (int i = 0; i < tokens.size(); i++) {
        delete tokens[i];
    }
}

const std::vector<Token *> &Command::get_tokens() const {
    return tokens;
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
#include <iostream>

#include "input.h"

#include "input_parser.h"
#include "util.h"


StringToken::StringToken(std::string s) {
    value.assign(s.begin(), s.end());
}

void StringToken::push_back(char c) {
    value.push_back(c);
}

std::string StringToken::to_str() const {
    return value;
}

std::string StringToken::get_raw() const {
    return value;
}

CommandName::CommandName(const TempToken &token) {
    value = token.get_raw();
}

VariableName::VariableName(const TempToken &token) {
    value = token.get_raw();
}


String::String() {

}

String::~String() {
    for (int i = 0; i < literals.size(); i++) {
        delete literals[i];
    }
}

//template<class T>
//void String::push_back(T token) {
//    static_assert(std::is_base_of<Token, T>::value, "Must be derived from Token");
//    T *t = new T;
//    *t = token;
//    literals.push_back(t);
//}

const std::vector<Token *>& String::get_vector() {
    return literals;
}

std::string String::format(std::map<std::string, std::string> variables) {
    return "";
}

std::string String::to_str() const {
    return "";
}


Command::Command(const std::vector<Token *> &tokens) {
    // TODO
//    this->tokens.assign(tokens.begin(), tokens.end());
}

Command::~Command() {

}


Input::Input() : istream(&std::cin) {

}

Input::Input(std::istream &in, std::ostream &out) : istream(&in), ostream(&out) {

}

Input::~Input() = default;


Command Input::next() {
    if (inputBuffer.empty()) {
        inputBuffer.push(read());
    }
    std::string input = inputBuffer.front();
    inputBuffer.pop();
    Command cmd = parse(input);
    return cmd;
}


bool Input::end() {
    if (inputBuffer.empty()) {
        inputBuffer.push(read());
    }
    return inputBuffer.front() == "quit";
}

std::string Input::read() {
    std::string input;
    sprint(*ostream, "prompt> ");
    getline(*istream, input);
    return input;
}


Command Input::parse(std::string const &input) {
    InputParser parser{input};

    throw InvalidCommandError();
}
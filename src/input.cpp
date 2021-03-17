
#include <iostream>
#include "input.h"


Input::Input() {

}

Input::~Input() {

}


Command Input::next() {
    if (currentInput.empty()) {
        currentInput = read();
    }
    Command cmd = parse(currentInput);
    currentInput.erase();
    return cmd;
}


bool Input::end() {
    if (currentInput.empty()) {
        currentInput = read();
    }
    return currentInput == "quit";
}

std::string Input::read() {
    std::string input;
    getline(std::cin, input);
    input = std::regex_replace(input, stripRegex, "$1");
    return input;
}


Command Input::parse(std::string input) {

}


#include <iostream>
#include "input.h"

// regex patterns
const std::regex Input::stripRegex = std::regex(R"(^\s*(.*?)\s*?$)");
const std::regex Input::argumentsRegex = std::regex(R"((("?)(?:\\[^\\]|[^\s"])+(\2)))");
const std::regex Input::commandRegex = std::regex(R"(^\s*?(\w+)(?:\s*(.*))?$)");
const std::regex Input::reduceEscapeCharRegex = std::regex(R"(([^\\]|^)\\)");
const std::regex Input::assignmentRegex = std::regex(R"(^(\w+)\s*?=\s*?(.*?)\s*?$)");


Input::Input() = default;

Input::~Input() = default;


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
    return strip(input);
}


Command Input::parse(std::string const &input) {
    // Transform variable assignment operation into common command
    std::smatch assignment;
    if (std::regex_search(input, assignment, assignmentRegex)) {
        CommandName name = assignment[1];
        std::vector<std::string> values = parseArgs(assignment[2]);
        if (values.size() > 1) {
            throw InvalidCommandError();
        } else if (values.empty()) {
            return Command("set", {name});
        } else {
            return Command("set", {name, values[0]});
        }
    } else { // Process a common command
        std::smatch command;
        if (std::regex_search(input, command, commandRegex)) {
            CommandName name = command[1];
            CommandArgs args = parseArgs(command[2]);
            return Command(name, args);
        }
    }

    throw InvalidCommandError();
}


std::string Input::strip(std::string const &input) {
    return std::regex_replace(input, stripRegex, "$1");
}

CommandArgs Input::parseArgs(std::string const &input) {
    CommandArgs result;

    std::sregex_iterator matches(input.begin(), input.end(), argumentsRegex);
    std::sregex_iterator end;

    for(; matches != end; matches++) {
        // save the 2nd capture group
        std::string value = (*matches)[1];
        value = std::regex_replace(value, reduceEscapeCharRegex, "$1");
        result.emplace_back(value);
    }

    return result;
}

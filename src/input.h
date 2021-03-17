
#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <utility>
#include <string>
#include <regex>

typedef std::string CommandName;
typedef std::pair<CommandName, std::string> Command;

class Input {
public:
    Input();
    ~Input();
    Command next();
    bool end();
private:
    std::string read();
    Command parse(std::string);
    std::string currentInput;

    // regex
    const std::regex stripRegex = std::regex(R"(\s*(.*)\s*)");
    const std::regex assignmentRegex = std::regex(R"((\w+?)\s*=\s*(.*)\s*)");
};


#endif //SHELL_INPUT_H

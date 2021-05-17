
#ifndef SHELL_INPUT_PARSER_H
#define SHELL_INPUT_PARSER_H

#include <vector>

#include "input.h"
#include "fsm.h"
#include "charset.h"


struct InvalidInput : public std::exception {};


struct InputTransition : public Transition {
    Charset charset;
};

class InputParser : public FSM {
public:
    explicit InputParser(std::string input);
    ~InputParser();

    Command run();
private:
    enum States : State {
        S_START = 0,
        S_SKIP_SEPARATOR = 1,
        S_TEMPTOKEN = 2,
        S_COMMAND = 3,
        S_ASSIGNMENT = 4,
        S_STRING = 5,
        S_LINK_FRAME = 6,
        S_LINK = 7,
        S_END = 8
    };
protected:
    std::string input;
    std::vector<Token *> tokens;
    unsigned int index;

    char get_char();
    std::optional<Transition> get_transition(State state) override;
    void run_transition_callback(const TransitionCallback &callback) override;

    // Transition callbacks
    void T_Skip();
    void T_Delegate();
    void T_AddTempToken();
    void T_AddString();
    void T_AddText();
    void T_AddLink();
    void T_AddAssignment();
    void T_AppendTempToken();
    void T_AppendText();
    void T_AppendLink();
    void T_CastTempToCommand();
    void T_CastTempToVariable();


    const InputTransition transitions[19] = {
            {S_SKIP_SEPARATOR, S_SKIP_SEPARATOR, {TC(T_Skip)},                                      SPACES},
            {S_SKIP_SEPARATOR, NextState(),      {TC(T_Delegate)},                                  CHARS},

            {S_START,          NextState(S_SKIP_SEPARATOR,true),  {TC(T_Delegate)},                 SPACES},
            {S_START,          S_TEMPTOKEN,      {TC(T_AddTempToken), TC(T_AppendTempToken)},       LETTERS + "_"},

            {S_TEMPTOKEN,      S_TEMPTOKEN,      {TC(T_AppendTempToken)},                           LETTERS + DIGITS + "_"},
            {S_TEMPTOKEN,      S_COMMAND,        {TC(T_CastTempToCommand), TC(T_Delegate)},         SPACES + LINE_END},
            {S_TEMPTOKEN,      S_ASSIGNMENT,     {TC(T_CastTempToVariable), TC(T_AddAssignment)},   "="},

            {S_COMMAND,        NextState(S_SKIP_SEPARATOR, true),  {TC(T_Delegate)},                SPACES},
            {S_COMMAND,        S_STRING,         {TC(T_AddString)},                                 "\""},
            {S_COMMAND,        S_END,            {TC(T_Skip)},                                      LINE_END},

            {S_ASSIGNMENT,     S_STRING,         {TC(T_AddString)},                                 "\""},

            {S_STRING,         S_LINK_FRAME,     {TC(T_Skip)},                                      "$"},
            {S_STRING,         S_END,            {TC(T_Skip)},                                      "\""},
            {S_STRING,         S_STRING,         {TC(T_AppendText)},                                CHARS + SPACES},

            {S_LINK_FRAME,     S_LINK,           {TC(T_AddLink)},                                   "{"},
            {S_LINK_FRAME,     S_STRING,         {TC(T_Delegate), TC(T_Delegate)},                  CHARS + SPACES},

            {S_LINK,           S_STRING,         {TC(T_Skip)},                                      "}"},
            {S_LINK,           S_LINK,           {TC(T_AppendLink)},                                LETTERS + DIGITS + "_"},

            {S_END,            NextState(),      {},                                                ""}
    };
};

#endif //SHELL_INPUT_PARSER_H

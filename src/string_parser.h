
#ifndef SHELL_STRING_PARSER_H
#define SHELL_STRING_PARSER_H



#include <vector>

#include "tokens.h"
#include "fsm.h"
#include "charset.h"


struct InvalidString : public std::exception {};


struct StringTransition : public Transition {
    Charset charset;
};

class StringParser : public FSM {
public:
    explicit StringParser(std::string input);
    ~StringParser();

    StringToken run();
private:
    enum States : State {
        S_START = 0,
        S_LINK_FRAME = 1,
        S_LINK = 2,
        S_END = 3
    };
protected:
    std::string input;
    std::vector<std::shared_ptr<Token>> tokens;
    unsigned int index;

    char get_char();
    std::optional<Transition> get_transition(State state) override;
    void run_transition_callback(const TransitionCallback &callback) override;

    // Transition callbacks
    void T_Skip();
    void T_Delegate();
    void T_AddText();
    void T_AddLink();
    void T_AppendText();
    void T_AppendLink();

    const StringTransition transitions[8] = {
            {S_START,      S_LINK_FRAME, {TC(T_Skip)},                     "$"},
            {S_START,      S_END,        {TC(T_Skip)},                     LINE_END},
            {S_START,      S_START,      {TC(T_AppendText)},               CHARS + SPACES},

            {S_LINK_FRAME, S_LINK,       {TC(T_AddLink)},                  "{"},
            {S_LINK_FRAME, S_START,      {TC(T_Delegate), TC(T_Delegate)}, CHARS + SPACES},

            {S_LINK,       S_START,      {TC(T_Skip)},                     "}"},
            {S_LINK,       S_LINK,       {TC(T_AppendLink)},               LETTERS + DIGITS + "_"},

            {S_END,        NextState(),  {},                               ""}
    };
};



#endif //SHELL_STRING_PARSER_H

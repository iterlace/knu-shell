
#ifndef SHELL_INPUT_PARSER_H
#define SHELL_INPUT_PARSER_H

#include <vector>

#include "input.h"
#include "fsm.h"


class InputParser {
public:
    InputParser(std::string input);
    ~InputParser();

    Command run();
protected:
    FSM<InputParser> *fsm;
    std::string input;
    std::vector<Token *> tokens;
    unsigned int index;

    char get_char();
    char get_char(int idx);

    // FSM States
    // The functions also perform transitions and run callbacks
    void ST_1_Separator();
    void ST_2_TempToken();
    void ST_3_Separator();
    void ST_4_AssignmentOperator();
    void ST_5_String();
    void ST_6_Link();
    void ST_7_Text();
    void ST_End();

    // Transition callbacks
    void TR_Skip();
    void TR_Delegate();
    void TR_AddTempToken();
    void TR_AddString();
    void TR_AddText();
    void TR_AddLink();
    void TR_AddAssignment();
    void TR_AppendTempToken();
    void TR_AppendText();
    void TR_AppendLink();
    void TR_CastTempToCommand();
    void TR_CastTempToVariable();

};

#endif //SHELL_INPUT_PARSER_H


#include "input_parser.h"

#include "fsm.cpp"
#include "util.h"


InputParser::InputParser(std::string input) {
    fsm = new FSM<InputParser>(this);
    this->input.assign(input.begin(), input.end());
    index = 0;
}

InputParser::~InputParser() {
    delete fsm;
}


Command InputParser::run() {
    fsm->push_state(&InputParser::ST_1_Separator);
    while (fsm->current_state() != nullptr) {
        fsm->update();
        index++;
    }

    return Command(tokens);
}


char InputParser::get_char() {
    return input[index];
}


char InputParser::get_char(int idx) {
    return input[idx];
}


// STATES

void InputParser::ST_1_Separator() {
    fsm->pop_state();

    if (get_char() == ' ') {
        NEXT_STATE(ST_1_Separator)
    } else {
        NEXT_STATE(ST_2_TempToken)
        // transition callbacks
        TR_AddTempToken();
        TR_AppendTempToken();
    }
}

void InputParser::ST_2_TempToken() {
    fsm->pop_state();

    if (get_char() == ' ') {
        NEXT_STATE(ST_3_Separator)
        // transition callbacks
        TR_CastTempToCommand();
    } else if (get_char() == '=') {
        NEXT_STATE(ST_4_AssignmentOperator)
        // transition callbacks
        TR_CastTempToVariable();
        TR_AddAssignment();
    } else {
        NEXT_STATE(ST_2_TempToken)
        TR_AppendTempToken();
    }
}

void InputParser::ST_3_Separator() {
    fsm->pop_state();

    if (get_char() == ' ') {
        NEXT_STATE(ST_3_Separator)
    } else {
        NEXT_STATE(ST_5_String)
        // transition callbacks
        TR_AddString();
    }
}

void InputParser::ST_4_AssignmentOperator() {
    fsm->pop_state();

    if (get_char() == '\"') {
        NEXT_STATE(ST_5_String)
        // transition callbacks
        TR_AddString();
    }
}

void InputParser::ST_5_String() {
    fsm->pop_state();

    if (get_char() == '\"' && get_char(index - 1) != '\\') {
        NEXT_STATE(ST_End)
    } else if (get_char(index) == '$' &&
               get_char(index + 1) == '{' &&
               get_char(index - 1) != '\\') {
        NEXT_STATE(ST_6_Link)
        // transition callbacks
        TR_AddLink();
    } else {
        NEXT_STATE(ST_7_Text)
        // transition callbacks
        TR_AddText();
        TR_AppendText();
    }
}

void InputParser::ST_6_Link() {
    fsm->pop_state();

    if (get_char() == '}') {
        NEXT_STATE(ST_5_String)
        // transition callbacks
        TR_Skip();
    } else if (get_char() == '$' || get_char() == '{') {
        NEXT_STATE(ST_6_Link)
        // transition callbacks
        TR_Skip();
    } else {
        NEXT_STATE(ST_6_Link)
        // transition callbacks
        TR_AppendLink();
    }
}

void InputParser::ST_7_Text() {
    fsm->pop_state();

    if (get_char() == '\"' && get_char(index - 1) != '\\') {
        NEXT_STATE(ST_5_String)
        // transition callbacks
        TR_Delegate();
    } else if (get_char(index) == '$' &&
               get_char(index + 1) == '{' &&
               get_char(index - 1) != '\\') {
        NEXT_STATE(ST_5_String)
        // transition callbacks
        TR_Delegate();
    } else {
        NEXT_STATE(ST_7_Text)
        // transition callbacks
        TR_AppendText();
    }
}


void InputParser::ST_End() {
    fsm->pop_state();
}



// TRANSITION CALLBACKS

void InputParser::TR_Skip() {

}

void InputParser::TR_Delegate() {
    index--;
}

void InputParser::TR_AddTempToken() {
    auto t = new TempToken();
    tokens.push_back(t);
}

void InputParser::TR_AddString() {
    auto t = new String();
    tokens.push_back(t);
}

void InputParser::TR_AddText() {
    if (auto s = dynamic_cast<String *>(tokens.back())) {
        s->push_back(Text());
    }
}

void InputParser::TR_AddLink() {
    if (auto s = dynamic_cast<String *>(tokens.back())) {
        s->push_back(Link());
    }
}

void InputParser::TR_AddAssignment() {
    // TODO
}

void InputParser::TR_AppendTempToken() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        t->push_back(get_char());
    }

}

void InputParser::TR_AppendText() {
    if (auto s = dynamic_cast<String *>(tokens.back())) {
        if (auto t = dynamic_cast<Text *>(s->get_vector().back())) {
            t->push_back(get_char());
        } else {
            TR_AddText();
            TR_AppendText();
        }
    }
}

void InputParser::TR_AppendLink() {
    if (auto s = dynamic_cast<String *>(tokens.back())) {
        if (auto l = dynamic_cast<Link *>(s->get_vector().back())) {
            l->push_back(get_char());
        } else {
            TR_AddLink();
            TR_AppendLink();
        }
    }
}

void InputParser::TR_CastTempToCommand() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        auto c = new CommandName(*t);
        delete t;
        tokens.back() = c;
    }
}

void InputParser::TR_CastTempToVariable() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        auto v = new VariableName(*t);
        delete t;
        tokens.back() = v;
    }
}



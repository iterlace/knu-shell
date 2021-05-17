#include <iostream>

#include "input_parser.h"

#include "fsm.h"
#include "util.h"


InputParser::InputParser(std::string input) {
    this->input.assign(input.begin(), input.end());
    this->input += LINE_END;
    index = 0;
}

InputParser::~InputParser() {

}


Command InputParser::run() {
    unsigned int input_length = input.length();

    push_state(S_START);

    while (index < input_length) {
        FSMResult update_status = update();
        if (update_status == OK) {
            index++;
        } else if (update_status == INVALID_STATE) {
//            std::cout << "InvalidInput(); Current State is " << int(current_state().value()) << std::endl;
            throw InvalidInput();
        } else if (update_status == END) {
            break;
        }
    }

    return Command(tokens);
}


char InputParser::get_char() {
    return input[index];
}

std::optional<Transition> InputParser::get_transition(State state) {
    char current_char = get_char();
    for (const InputTransition& transition : transitions) {
        if (transition.state != state)
            continue;
        if (transition.charset == ANY_CHAR)
            return transition;
        if (transition.charset.find(current_char) != std::string::npos)
            return transition;
    }
    return std::nullopt;
}


void InputParser::run_transition_callback(const TransitionCallback &callback) {
    (this->*callback)();
}



// TRANSITION CALLBACKS

void InputParser::T_Skip() {

}

void InputParser::T_Delegate() {
    index--;
}

void InputParser::T_AddTempToken() {
    auto t = new TempToken();
    tokens.push_back(t);
}

void InputParser::T_AddString() {
    auto t = new StringToken();
    tokens.push_back(t);
}

void InputParser::T_AddText() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back())) {
        s->push_back(TextToken());
    }
}

void InputParser::T_AddLink() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back())) {
        s->push_back(LinkToken());
    }
}

void InputParser::T_AddAssignment() {
    auto t = new AssignmentToken();
    tokens.push_back(t);
}

void InputParser::T_AppendTempToken() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        t->push_back(get_char());
    }

}

void InputParser::T_AppendText() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back())) {
        if (s->get_vector().empty()) {
            T_AddText();
        }
        if (auto t = dynamic_cast<TextToken *>(s->get_vector().back())) {
            t->push_back(get_char());
        } else {
            T_AddText();
            T_AppendText();
        }
    }
}

void InputParser::T_AppendLink() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back())) {
        if (auto l = dynamic_cast<LinkToken *>(s->get_vector().back())) {
            l->push_back(get_char());
        } else {
            T_AddLink();
            T_AppendLink();
        }
    }
}

void InputParser::T_CastTempToCommand() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        auto c = new CommandToken(*t);
        delete t;
        tokens.back() = c;
    }
}

void InputParser::T_CastTempToVariable() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back())) {
        auto v = new VariableToken(*t);
        delete t;
        tokens.back() = v;
    }
}



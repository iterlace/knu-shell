#include <iostream>

#include "input_parser.h"

#include "fsm.h"
#include "util.h"


InputParser::InputParser(std::string input) {
    this->input.assign(input.begin(), input.end());
    this->input += "\n";
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
        if (transition.charset.contains(current_char))
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
    auto t = std::make_shared<TempToken>();
    tokens.push_back(t);
}

void InputParser::T_AddString() {
    auto t = std::make_shared<StringToken>();
    tokens.push_back(t);
}

void InputParser::T_AddText() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back().get())) {
        s->push_back(TextToken());
    }
}

void InputParser::T_AddLink() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back().get())) {
        s->push_back(LinkToken());
    }
}

void InputParser::T_AddAssignment() {
    auto t = std::make_shared<AssignmentToken>();
    tokens.push_back(t);
}

void InputParser::T_AppendTempToken() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back().get())) {
        t->push_back(get_char());
    }

}

void InputParser::T_AppendText() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back().get())) {
        if (s->get_vector().empty()) {
            T_AddText();
        }
        if (auto t = dynamic_cast<TextToken *>(s->get_vector().back().get())) {
            t->push_back(get_char());
        } else {
            T_AddText();
            T_AppendText();
        }
    }
}

void InputParser::T_AppendLink() {
    if (auto s = dynamic_cast<StringToken *>(tokens.back().get())) {
        if (auto l = dynamic_cast<LinkToken *>(s->get_vector().back().get())) {
            l->push_back(get_char());
        } else {
            T_AddLink();
            T_AppendLink();
        }
    }
}

void InputParser::T_CastTempToCommand() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back().get())) {
        auto c = std::make_shared<CommandToken>(*t);
        tokens.pop_back();
        tokens.push_back(c);
    }
}

void InputParser::T_CastTempToVariable() {
    if (auto t = dynamic_cast<TempToken *>(tokens.back().get())) {
        auto v = std::make_shared<VariableToken>(*t);
        tokens.pop_back();
        tokens.push_back(v);
    }
}



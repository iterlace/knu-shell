
#include "string_parser.h"
#include "fsm.h"
#include "util.h"


StringParser::StringParser(std::string input) {
    this->input.assign(input.begin(), input.end());
    this->input += LINE_END;
    index = 0;
}

StringParser::~StringParser() {

}


StringToken StringParser::run() {
    unsigned int input_length = input.length();

    push_state(S_START);

    while (index < input_length) {
        FSMResult update_status = update();
        if (update_status == OK) {
            index++;
        } else if (update_status == INVALID_STATE) {
            throw InvalidString();
        } else if (update_status == END) {
            break;
        }
    }

    return StringToken(tokens);
}


char StringParser::get_char() {
    return input[index];
}

std::optional<Transition> StringParser::get_transition(State state) {
    char current_char = get_char();
    for (const StringTransition &transition : transitions) {
        if (transition.state != state)
            continue;
        if (transition.charset == ANY_CHAR)
            return transition;
        if (transition.charset.contains(current_char))
            return transition;
    }
    return std::nullopt;
}


void StringParser::run_transition_callback(const TransitionCallback &callback) {
    (this->*callback)();
}



// TRANSITION CALLBACKS

void StringParser::T_Skip() {

}

void StringParser::T_Delegate() {
    index--;
}

void StringParser::T_AddText() {
    auto t = new TextToken();
    tokens.push_back(t);
}

void StringParser::T_AddLink() {
    auto t = new LinkToken();
    tokens.push_back(t);
}


void StringParser::T_AppendText() {
    if (tokens.empty()) {
        T_AddText();
    }
    if (auto t = dynamic_cast<TextToken *>(tokens.back())) {
        t->push_back(get_char());
    } else {
        T_AddText();
        T_AppendText();
    }
}

void StringParser::T_AppendLink() {
    if (auto l = dynamic_cast<LinkToken *>(tokens.back())) {
        l->push_back(get_char());
    } else {
        T_AddLink();
        T_AppendLink();
    }
}

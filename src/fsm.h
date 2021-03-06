
#ifndef SHELL_FSM_H
#define SHELL_FSM_H

#include <stack>
#include <vector>
#include <optional>
#include <set>

#include "util.h"

class FSM;
struct Transition;
class NextState;
typedef void (FSM::*TransitionCallback)();
typedef unsigned char State;


enum FSMResult {
    OK,
    END,
    INVALID_STATE
};


class FSM {
public:
protected:
    std::stack<State> stack;

    FSMResult update();
    void push_state(State);
    std::optional<State> pop_state();
    std::optional<State> current_state();

    virtual std::optional<Transition> get_transition(State state) = 0;
    void run_transition(const Transition &transition);
    virtual void run_transition_callback(const TransitionCallback &callback) = 0;
};


class NextState {
public:
    NextState();
    NextState(State next_state);
    NextState(State next_state, bool stack);

    std::optional<State> get() const;
    bool keep_stack() const;
protected:
    std::optional<State> next;
    bool stack;
};


struct Transition {
    State state;
    NextState next_state;
    std::vector<TransitionCallback> callbacks;
};


#define TC(method) \
static_cast<TransitionCallback>(&CLASS::method)


#endif //SHELL_FSM_H


#include "fsm.h"




FSMResult FSM::update() {
    std::optional<State> state = current_state();
    if (!state.has_value()) {
        return END;
    }

    // Go to next state
    std::optional<Transition> transition = get_transition(state.value());
    if (!transition.has_value())
        return INVALID_STATE;
    run_transition(transition.value());
    return OK;
}


void FSM::push_state(State state) {
    auto current = current_state();
    if (current.has_value() && current.value() == state)
        return;

    stack.push(state);
}


std::optional<State> FSM::pop_state() {
    if (stack.empty()) {
        return std::nullopt;
    }
    auto item = stack.top();
    stack.pop();
    return item;
}


std::optional<State> FSM::current_state() {
    if (stack.empty()) {
        return std::nullopt;
    }
    return stack.top();
}


void FSM::run_transition(const Transition &transition) {
    if (!transition.next_state.keep_stack()) {
        pop_state();
    }

    for (auto callback : transition.callbacks) {
        run_transition_callback(callback);
    }

    std::optional<State> next = transition.next_state.get();
    if (next.has_value()) {
        stack.push(next.value());
    }
}


NextState::NextState() {
    next = std::nullopt;
    stack = false;
}


NextState::NextState(State next_state) {
    next = next_state;
    stack = false;
}


NextState::NextState(State next_state, bool stack) {
    next = next_state;
    this->stack = stack;
}


std::optional<State> NextState::get() const {
    return next;
}

bool NextState::keep_stack() const {
    return stack;
}

//template class FSM<InputParser>;

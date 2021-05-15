
#include "fsm.h"



template<class T>
FSM<T>::FSM(T *client) {
    this->client = client;
}


template<class T>
void FSM<T>::update() {
    StateFn state = current_state();
    if (state != nullptr) {
        (client->*state)();
    }
}


template<class T>
void FSM<T>::push_state(StateFn state) {
    if (current_state() != state) {
        stack.push(state);
    }
}


template<class T>
typename FSM<T>::StateFn FSM<T>::pop_state() {
    auto item = stack.top();
    stack.pop();
    return item;
}


template<class T>
typename FSM<T>::StateFn FSM<T>::current_state() {
    if (stack.empty()) {
        return nullptr;
    }
    return stack.top();
}

//template class FSM<InputParser>;

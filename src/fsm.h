
#ifndef SHELL_FSM_H
#define SHELL_FSM_H

#include <stack>

#include "util.h"


template<class T>
class FSM {
public:
    typedef void (T::*StateFn)();

    FSM(T *client);

    void update();

    void push_state(StateFn);

    StateFn pop_state();

    StateFn current_state();

private:
    T *client;
    std::stack<StateFn> stack;
};


#define NEXT_STATE(method) \
fsm->push_state(&CLASS::method);


#endif //SHELL_FSM_H

#pragma once
#include "defs.h"
#include <list>
using std::list;

class State {
    public:
        State();
        f32 time;
        f32 deltaTime;
        void updateState();
};

#pragma once
#include "defs.h"

class State {
    public:
        State();
        f32 time;
        f32 deltaTime;
        void updateState();
};

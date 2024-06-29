#include "state.hpp"
#include <stdlib.h>
#include "raylib.h"



State::State(){
    this->deltaTime = 0;
    this->time = 0;
}

void State::updateState(){
    f32 deltaTime = GetFrameTime();
    time += deltaTime;
}

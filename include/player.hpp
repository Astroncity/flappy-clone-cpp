#pragma once
#include <stdlib.h>
#include "raylib.h"
#include "space.hpp"

class Player{
    public:
        Player();
        void drawPlayer();
        void update();
        
        i32 score = 0;
        Space* space;
        Rectangle rect;
        Texture2D texture;
        Vector2 velocity;
        bool frozen = false;
    private:
        void handleCollision();
};

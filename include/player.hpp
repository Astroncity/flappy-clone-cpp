#include <stdlib.h>
#include "raylib.h"
#include "space.hpp"

class Player{
    public:
        Player();
        void drawPlayer();
        void update();
        

        Space* space;
        Rectangle rect;
        Texture2D texture;
        Vector2 velocity;
};

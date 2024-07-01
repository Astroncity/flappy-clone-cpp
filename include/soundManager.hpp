#pragma once
#include "raylib.h"

class SoundManager{
    public:
        static Sound background[2];
        static Sound death;
        static Sound flap;
        static Sound score;
        static Sound start;
        static int currentBackground;

        static void handleBackgroundMusic();
        static void init();
};

#pragma once
#include "defs.h"
#include <random>
#include "player.hpp"

class PipeHandler{
    public:
        PipeHandler(const f64 pipeCooldown, const f32 scrollSpeed);
        void runPipes(Player* player);
        void drawPipes();

        f64 timer;
        const f64 pipeCooldown;
        f32 scrollSpeed;
        std::random_device rand;
};

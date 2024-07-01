#pragma once
#include "raylib.h"
#include <list>
using std::list;
#include "defs.h"
#include <stdbool.h>
#include "player.hpp"

#define MARKED_FOR_DELETION 69

class Pipe{
    static inline const char* imgPath = "images/pipe-green.png";

    public:
        Pipe(f32 x, f32 toph, f32 both, f32 gap, Player* player);
        void draw();
        void editPipe(f32 toph, f32 both, f32 gap);
        i32 scroll(f32 speed); 
        Rectangle* getColliders();
        static inline list<Pipe*>* pipes = nullptr;

        Texture2D topTex;
        Texture2D botTex;
        Rectangle topRec;
        Rectangle botRec;
        f32 both;
        f32 toph;
        f32 gap;
        f32 x;
        Player* player;
        bool scored = false;
    private:
        void score();
};

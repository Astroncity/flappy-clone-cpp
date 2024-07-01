#include "pipe.hpp"
#include "defs.h"
#include "raylib.h"
#include "soundManager.hpp"
#include <stdlib.h>
Texture2D baseTex;
bool loadedTex = false;

void validatePipe(){
    if(!Pipe::pipes){
        Pipe::pipes = new list<Pipe*>();
    }
    
    if(!loadedTex){
        baseTex = LoadTextureFromImage(LoadImage("images/pipe-green.png"));
        loadedTex = true;
    }
}



Pipe::Pipe(f32 x, f32 toph, f32 both, f32 gap, Player* player){
    validatePipe();
    this->player = player;

    topTex = baseTex;
    botTex = topTex;
    
    toph -= gap;
    both -= gap;
    this->toph = toph;
    this->both = both;
    this->x = x;

    const i32 sh = 512;
    const f32 originOffset = topTex.height/2.0;
    
    topRec = {x, -originOffset + toph, (float)baseTex.width, (float)baseTex.height};
    botRec = {x, sh + originOffset - both, (float)baseTex.width, (float)baseTex.height};
    
    pipes->push_back(this);
}



Rectangle* Pipe::getColliders(){
    Rectangle newTop = topRec;
    Rectangle newBottom = botRec;

    newTop.x -= 26;
    newBottom.x -= 26;
    newTop.y -= 160;
    newBottom.y -= 160;
    Rectangle* rects = (Rectangle*)malloc(sizeof(Rectangle) * 2);
    rects[0] = newTop;
    rects[1] = newBottom;
    return rects;
}


void Pipe::draw(){
    const f32 w = topTex.width;
    const f32 h = topTex.height;
    Rectangle srcStd = {0, 0, w, h};
    Rectangle srcMirror = {w, 0, -w, h};
    DrawTexturePro(topTex, srcMirror, topRec,(Vector2){w/2, h/2}, 180, WHITE);
    DrawTexturePro(botTex, srcStd, botRec, (Vector2){w/2, h/2}, 0, WHITE);
    
    //Rectangle* rects = getColliders();

    //DrawRectangleRec(rects[0], GREEN);
    //DrawRectangleRec(rects[1], RED);
}


void Pipe::score(){
    if(x < player->space->pos.x && !scored){
        player->score++;
        scored = true;
        PlaySound(SoundManager::score);
    }
}


i32 Pipe::scroll(f32 speed){
    
    const i32 sh = 512;
    const f32 originOffset = topTex.height/2.0;
    const f32 w = topTex.width;
    const f32 h = topTex.height;

    x -= speed * GetFrameTime();

    topRec = {x, -originOffset + toph, w, h};
    botRec = {x, sh + originOffset - both, w, h};

    score();


    if(x < -topTex.width/2.0){
        return MARKED_FOR_DELETION;
    }
    else{
        return 0;
    }
}


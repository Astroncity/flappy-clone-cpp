#include "player.hpp"
#include "pipe.hpp"
#include "raylib.h"
#include "soundManager.hpp"


const char* birdPngPath = "images/bird.png";



static inline void physicsHandler(Player* plr){
    if(plr->frozen) return;

    if(plr->rect.y > 0){
        if(IsKeyPressed(KEY_SPACE)){
            plr->velocity.y = -2;
            PlaySound(SoundManager::flap);
        }
    }
    else{
        plr->velocity.y = 2;
    }
    // gravity, 
    plr->velocity.y += 6 * GetFrameTime();
    
    plr->space->pos.x += plr->velocity.x;
    plr->space->pos.y += plr->velocity.y;
}


static inline void printPlayerDebug(Player* plr){
    printf("Player at (%.0f, %.0f)\n", plr->space->pos.x, plr->space->pos.y);
}


Player::Player(){
    space = new Space();
    rect.width = 34 * 0.8f;
    rect.height = 24 * 0.8f;
    
    velocity = (Vector2){0,0};
    texture = LoadTextureFromImage(LoadImage(birdPngPath));
}

void Player::drawPlayer(){
    //DrawRectangleRec(this->rect, col);
    Rectangle src = {0, 0, 34, 24};
    DrawTextureRec(texture, src, space->pos, WHITE);
    //DrawRectangleRec(rect, (Color){0, 255, 0, 150});
}

void Player::update(){
    physicsHandler(this);
    rect.x = space->pos.x + 2.5;
    rect.y = space->pos.y + 2;
    //printPlayerDebug(this);
    handleCollision();
}


void Player::handleCollision(){
    if(frozen) return;
    list<Pipe*>* pipes = Pipe::pipes;
    if(!pipes) return;
    auto iter = pipes->begin();

    for(; iter != pipes->end(); iter++){
        Pipe* pipe = *iter;
        Rectangle* rects = pipe->getColliders();

        if(CheckCollisionRecs(rect, rects[0])){
            frozen = true;
            PlaySound(SoundManager::death);
        }
        else if(CheckCollisionRecs(rect, rects[1])){
            PlaySound(SoundManager::death);
            frozen = true;
        }

        free(rects);
    }
}

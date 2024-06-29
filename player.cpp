#include "player.hpp"


const char* birdPngPath = "images/bird.png";


static inline void physicsHandler(Player* plr){
    if(IsKeyPressed(KEY_SPACE)){
        plr->velocity.y -= 2;
    }
    // gravity, 
    plr->velocity.y += 2 * GetFrameTime();
    

    plr->space->pos.x += plr->velocity.x;
    plr->space->pos.y += plr->velocity.y;
}


static inline void printPlayerDebug(Player* plr){
    printf("Player at (%.0f, %.0f)\n", plr->space->pos.x, plr->space->pos.y);
}


Player::Player(){
    this->space = new Space();
    this->rect.width = 34;
    this->rect.height = 24;
    this->velocity = (Vector2){0,0};
    this->texture = LoadTextureFromImage(LoadImage(birdPngPath));
}

void Player::drawPlayer(){
    //DrawRectangleRec(this->rect, col);
    Rectangle src = {0, 0, 34, 24};
    DrawTextureRec(this->texture, src, this->space->pos, WHITE);
}

void Player::update(){
    physicsHandler(this);
    this->rect.x = this->space->pos.x;
    this->rect.y = this->space->pos.y;
    printPlayerDebug(this);
}




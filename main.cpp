#include "defs.h"
#include "raylib.h"
#include "state.hpp"
#include "player.hpp"
#include "pipeHandler.hpp"

const char* backgroundPath = "images/background-day.png";
const char* floorPath = "images/base.png";
Texture2D backgroundPNG;
Texture2D floorPNG;


f32 floorOffset = 0;
f32 cityOffset = 0;
f32 globalScrollSpeed = 100;

void drawBackground(){
    // Background image is too narrow to cover entire screen
    // so we'll draw it a few times over

    const i32 texWidth = 288;

    for(int i = 0; i < 4; i++){
        DrawTexture(backgroundPNG, i * texWidth - cityOffset, 0, WHITE);
    }

    cityOffset += globalScrollSpeed/5 * GetFrameTime();
    if(cityOffset >= 288){
        cityOffset = 0;
    }
}


void drawFloor(){
    for(int i = 0; i < 4; i++){
        DrawTexture(floorPNG, i * 336 - floorOffset, 512 - 112, WHITE);
    }

    floorOffset += globalScrollSpeed * GetFrameTime();
    if(floorOffset >= 336){
        floorOffset = 0;
    }
}


void checkStartGame(Player* plr, bool* gameStarted){
    if(!*gameStarted){
        if(IsKeyPressed(KEY_SPACE)){
            *gameStarted = true;
            plr->frozen = false;
        }
    }
}


void checkFloorCol(Player* plr){
    if(CheckCollisionRecs((Rectangle){0, 512 - 112, 800, 112}, plr->rect)){
        plr->frozen = true;
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 512;
    bool gameStarted = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Test Game");
    SetTargetFPS(144);
    State* state = new State();
    Player* player = new Player();
    player->space->pos = (Vector2){300, 100};
    player->frozen = true;

    backgroundPNG = LoadTextureFromImage(LoadImage(backgroundPath));
    floorPNG = LoadTextureFromImage(LoadImage(floorPath));
    PipeHandler* pipeHandler = new PipeHandler(3, globalScrollSpeed);

    while(!WindowShouldClose()){
        state->updateState();
        player->update();
        checkFloorCol(player);
        pipeHandler->runPipes();
        checkStartGame(player, &gameStarted);
        BeginDrawing();
        
        ClearBackground(WHITE);
        drawBackground();
        
        pipeHandler->drawPipes();
        DrawText(TextFormat("Time: %.2f", state->time), 0, 0, 20, BLACK);
        player->drawPlayer();
        drawFloor();
        EndDrawing();
        
    }
}




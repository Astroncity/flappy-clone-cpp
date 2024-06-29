#include "defs.h"
#include "raylib.h"
#include "state.hpp"
#include "player.hpp"


const char* backgroundPath = "images/background-day.png";

Texture2D backgroundPNG;


void drawBackground(){
    // Background image is too narrow to cover entire screen
    // so we'll draw it a few times over

    const i32 texWidth = 288;

    for(int i = 0; i < 2; i++){
        DrawTexture(backgroundPNG, i * texWidth, 0, WHITE);
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 512;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Test Game");
    SetTargetFPS(60);
    State* state = new State();
    Player* player = new Player();
    player->space->pos = (Vector2){screenHeight/2.0f, screenWidth/2.0f};

    LoadTextureFromImage(LoadImage(backgroundPath));


    while(!WindowShouldClose()){
        state->updateState();
        player->update();
        BeginDrawing();
        
        ClearBackground(WHITE);
        drawBackground();
        DrawText(TextFormat("Time: %.2f", state->time), 0, 0, 20, BLACK);
        player->drawPlayer();
        EndDrawing();
        
    }
}


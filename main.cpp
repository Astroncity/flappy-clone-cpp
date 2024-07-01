#include "defs.h"
#include "raylib.h"
#include "state.hpp"
#include "player.hpp"
#include "pipeHandler.hpp"
#include "pipe.hpp"
#include "soundManager.hpp"

#define MAX(a, v) ((a) > (v) ? (a) : (v))
#define MIN(a, v) ((a) < (v) ? (a) : (v))


const char* backgroundPath = "images/background-day.png";
const char* floorPath = "images/base.png";
Texture2D backgroundPNG;
Texture2D floorPNG;


f32 floorOffset = 0;
f32 cityOffset = 0;
f32 globalScrollSpeedDef = 100;
f32 globalScrollSpeed = 100;
Vector2 mouse;

Font bestFont;

Vector2 v2Clamp(Vector2 vec, Vector2 min, Vector2 max){
    return (Vector2){MIN(MAX(vec.x, min.x), max.x), MIN(MAX(vec.y, min.y), max.y)};
}

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

void cleanPipes();
void checkStartGame(Player* plr, bool* started){
    if(!*started){
        if(IsKeyPressed(KEY_SPACE)){
            *started = true;
            plr->frozen = false;
            PlaySound(SoundManager::start);
        }
    }
    else if(plr->frozen){
        if(IsKeyPressed(KEY_SPACE)){
            plr->score = 0;
            plr->space->pos = (Vector2){300, 100};
            plr->velocity.y = 0;
            cleanPipes();
            *started = false;
        }
    }
}


void cleanPipes(){
   for(auto &pipe:*Pipe::pipes){
       delete pipe;
   }    
   Pipe::pipes->clear();
}


void checkFloorCol(Player* plr){
    if(CheckCollisionRecs((Rectangle){0, 512 - 112, 800, 112}, plr->rect)){
        plr->frozen = true;
        PlaySound(SoundManager::death);
    }
}


void DrawText3D(const char* text, i32 x, i32 y, i32 size, Color color){
    for(usize i = 0; i < 5; i++){
        i16 tempA = (i16)color.a;
        tempA -= i * 50;
        if(tempA < 0) tempA = 0;

        Color cl = (Color){color.r, color.g, color.b, (u8)tempA};
        DrawTextEx(bestFont, text, (Vector2){(f32)x, y + (f32)i*2}, size, 1, cl);
    }
}


void drawStartingText(const i32 screenWidth){
    const char* text = "Press SPACE to start";
    i32 len = MeasureTextEx(bestFont, text, 80, 1).x;
    i32 x = (screenWidth - len) / 2;
    DrawText3D("Press SPACE to start", x, 50, 80, (Color){211, 211, 211, 200});
}


void drawScore(const i32 screenWidth, i32 score, bool frozen){
    const char* text = TextFormat("%i", score);
    i32 len = MeasureTextEx(bestFont, text, 150, 1).x;
    i32 x = (screenWidth - len) / 2;
    Color cl = (Color){211, 211, 211, 100};
    if(frozen) cl = (Color){253, 253, 150, 240};
    DrawText3D(text, x, 50, 150, cl);
}

int main(void) {
    const i32 screenWidth = 800;
    const i32 screenHeight = 512;
    bool started = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Test Game");
    InitAudioDevice();
    SetMasterVolume(0);
    SoundManager::init();
    SetTargetFPS(144);
    State* state = new State();
    Player* player = new Player();
    player->space->pos = (Vector2){300, 100};
    player->frozen = true;

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    bestFont = LoadFontEx("fonts/spaceMono.ttf", 100, NULL, 0);
    backgroundPNG = LoadTextureFromImage(LoadImage(backgroundPath));
    floorPNG = LoadTextureFromImage(LoadImage(floorPath));
    PipeHandler* pipeHandler = new PipeHandler(2, globalScrollSpeed);

    while(!WindowShouldClose()){
        state->updateState();
        player->update();
        
        f32 scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        Vector2 mouseOLD = GetMousePosition();
        mouse.x = (mouseOLD.x - (GetScreenWidth() - (screenWidth*scale))*0.5f)/scale;
        mouse.y = (mouseOLD.y - (GetScreenHeight() - (screenHeight*scale))*0.5f)/scale;
        mouse = v2Clamp(mouse, (Vector2){ 0, 0 }, (Vector2){ (f32)screenWidth, (f32)screenHeight });
        

        if(player->frozen){
            pipeHandler->scrollSpeed = 0;
            globalScrollSpeed = 0;
        }
        else{
            pipeHandler->scrollSpeed = globalScrollSpeed;
            globalScrollSpeed = globalScrollSpeedDef;
        }

        globalScrollSpeedDef += GetFrameTime() * 3;

        checkFloorCol(player);
        pipeHandler->runPipes(player);
        checkStartGame(player, &started);

        BeginTextureMode(target);
        
        ClearBackground(WHITE);
        drawBackground();
        if(started){
            drawScore(screenWidth, player->score, player->frozen);
            if(!player->frozen) SoundManager::handleBackgroundMusic();
            else{
                StopSound(SoundManager::background[SoundManager::currentBackground % 2]);
            }
        }
        pipeHandler->drawPipes();
        player->drawPlayer();
        drawFloor();

        if(!started) drawStartingText(screenWidth);

        EndTextureMode();

        BeginDrawing();
            ClearBackground(WHITE);     

            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)screenWidth*scale))*0.5f, (GetScreenHeight() - ((float)screenHeight*scale))*0.5f,
                           (float)screenWidth*scale, (float)screenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, Color{255,255,255,255});
        EndDrawing();

        
    }
}




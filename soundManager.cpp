#include "soundManager.hpp"
#include "raylib.h"


Sound SoundManager::background[2];
Sound SoundManager::death;
Sound SoundManager::flap;
Sound SoundManager::score;
Sound SoundManager::start;
int SoundManager::currentBackground = 0;

void SoundManager::init(){

    SoundManager::background[0] = LoadSound("sounds/background1.mp3");
    SoundManager::background[1] = LoadSound("sounds/background2.mp3");
    SoundManager::death = LoadSound("sounds/death.mp3");
    SoundManager::flap = LoadSound("sounds/flap.mp3");
    SoundManager::score = LoadSound("sounds/score.wav");
    SoundManager::start = LoadSound("sounds/start.wav");
    SoundManager::currentBackground = 0;

    SetSoundVolume(background[0], 0.5f);
    SetSoundVolume(background[1], 0.5f);
    
    SetSoundVolume(score, 0.5f);
}

void SoundManager::handleBackgroundMusic(){
    if(IsSoundPlaying(background[currentBackground % 2])) return;
    currentBackground++;
    PlaySound(background[currentBackground % 2]);
}

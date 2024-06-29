#include "pipeHandler.hpp"
#include "pipe.hpp"
#include <iostream>
#include <random>

PipeHandler::PipeHandler(const f64 pipeCooldown, const f32 scrollSpeed)
    : pipeCooldown(pipeCooldown),
      scrollSpeed(scrollSpeed){}


void PipeHandler::runPipes(){
    if(!Pipe::pipes){
        Pipe::pipes = new list<Pipe*>();
    }

    list<Pipe*>* pipes = Pipe::pipes;
    list<Pipe*>::iterator iter = pipes->begin();

    timer += GetFrameTime();

    for(; iter != pipes->end(); iter++){
        Pipe* p = *iter;
        i32 code = p->scroll(scrollSpeed);
        if(code == MARKED_FOR_DELETION){
            delete p;
            iter = pipes->erase(iter);
        }
    }


    if(timer > pipeCooldown){
        timer = 0;
        std::mt19937_64 gen(rand());
        std::uniform_int_distribution<> dis(200, 412);

        i32 remainder;
        i32 topH = dis(gen);
        remainder = 512 - topH;
        i32 widthOffset = 26;
        dis = std::uniform_int_distribution<>(30, 50);
        i32 gap = dis(gen);
    
        new Pipe(800 + widthOffset, remainder, topH, gap);
    }
}

void PipeHandler::drawPipes(){
    list<Pipe*>* pipes = Pipe::pipes;
    list<Pipe*>::iterator iter = pipes->begin();

    for(; iter != pipes->end(); iter++){ (*iter)->draw(); }

}

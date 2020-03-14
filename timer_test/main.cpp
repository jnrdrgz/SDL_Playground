#include <SDL2/SDL.h>
#include "game.h"
//#include "text.h"
#include "../log_system/log_system.h"

static LogSystem log_system = LogSystem();

class Timer
{
    public:
        Timer(){
            this->startTime = 0;
            this->pausedTime = 0;
            this->started = false;
            this->paused = false;
        }

        void start(){
            started = true;
            paused = false;
            startTime = SDL_GetTicks();
            pausedTime = 0; 
        }

        void stop(){
            started = false;
            paused = false;
            startTime = 0;
            pausedTime = 0;
        }

        void pause(){
            if(started && !paused){
                started = false;
                paused = true;
                startTime = 0;
                pausedTime = SDL_GetTicks() - startTime;
            }
        }

        void unpause(){
            if(started && paused){
                paused = false;
                startTime = SDL_GetTicks() - pausedTime;
                pausedTime = 0;  
            }
        }

        Uint32 getTicks(){
            Uint32 time = 0;
            
            if(started){
                if(paused){
                    time = pausedTime;
                }else{
                    time = SDL_GetTicks()-startTime;
                }

            }

            return time;
        }

        Uint32 getTicks_in_seconds(){
            return getTicks()/1000;
        }

        bool isStarted(){ return started; }
        bool isPaused(){ return paused && started; }

    private:
        Uint32 startTime;
        Uint32 pausedTime;
        bool paused;
        bool started;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Uint32 t = SDL_GetTicks();
    printf("%d\n", t);
    Timer timer;
    
    log_system.add_text("fps", std::to_string(t), game.renderer);

    while(game.running){
        t = SDL_GetTicks();
        //printf("%d\n", t);

        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_s){
                    timer.start();
                } else if(game.event.key.keysym.sym == SDLK_p){
                    timer.pause();
                }
            }
        }

        
        log_system.update_text("fps", std::to_string(timer.getTicks_in_seconds()), game.renderer);
        log_system.draw(game.renderer);

        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    game.close();
}

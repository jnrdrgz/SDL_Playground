#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();

struct Block
{
public:
    Block(){}
    Block(int x, int y, int w, int h, SDL_Color color){

    }
    
    SDL_Rect rct;
};

struct Pad
{
public:
    Pad(){}
    Pad(int x, int y, int w, int h){

    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rct);    
    }

    void update(){

    }

    SDL_Rect rct;
};

struct Ball
{
public:
    Ball(){}
    Ball(int x, int y, int w, int h){

    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rct);    
    }

    SDL_Rect rct;
};

struct BlocksGame
{
public:
    BlocksGame(int x, int y, int w, int h){
        delimiter.x = x;
        delimiter.y = y;
        delimiter.w = w;
        delimiter.h = h;

        pad.rct.w = delimiter.w/10;
        pad.rct.h = delimiter.h/20;
        pad.rct.x = (delimiter.x + delimiter.x/2);
        pad.rct.y = (delimiter.y+delimiter.h) - pad.rct.h;

        ball.rct.w = delimiter.w/30;
        ball.rct.h = ball.rct.w;
        ball.rct.x = pad.rct.x;
        ball.rct.y = pad.rct.y - ball.rct.h;
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &delimiter);
        pad.draw(renderer);
        ball.draw(renderer);
    }

    Pad pad;
    Ball ball;
    std::vector<Block> blocks;
    SDL_Rect delimiter;

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    BlocksGame blocksGame(100,100,300,300);
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                
                }
            }
        }

        blocksGame.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

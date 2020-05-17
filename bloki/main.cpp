#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"
#include <vector>

static LogSystem log_system = LogSystem();

bool rct_collide(SDL_Rect a, SDL_Rect b){
    if( a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y)
    {
        return true;
    }

    return false;

}

struct Block
{
public:
    Block(){
        active = true;
    }

    Block(int x, int y, int w, int h, SDL_Color color){
        active = true;

        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        color = color;
    }

    void draw(SDL_Renderer* renderer){
        if(active){
            SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(renderer, &rct);    
        }
    }
    
    SDL_Rect rct;
    SDL_Color color;
    bool active;
};

struct Pad
{
public:
    Pad(){
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }
    Pad(int x, int y, int w, int h){
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rct);    
    }

    void update(){
        rct.x += velocity.x;
        //rct.y += velocity.y;
    }

    SDL_Rect rct;
    Vector2 velocity;
};

struct Ball
{
public:
    Ball(){
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }
    Ball(int x, int y, int w, int h){

    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rct);    
    }

    void update(){
        rct.x += velocity.x;
        rct.y += velocity.y;
    }

    SDL_Rect rct;
    Vector2 velocity;

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

        int bx = delimiter.x + 20;
        int by = delimiter.y + 20;
        int bw = delimiter.w / 20;
        int bh = delimiter.h / 40;
        for(int i = 0; i < 10; i++){
            SDL_Color color = {255,0,0};
            blocks.push_back(Block(bx,by,bw,bh,color));
            bx += bw + 5;
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &delimiter);
        
        for(auto block : blocks){
            block.draw(renderer);
        }

        pad.draw(renderer);
        ball.draw(renderer);

    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_s){
                ball.velocity.x = 2.0f;
                ball.velocity.y = -2.0f;
            }
            if(event.key.keysym.sym == SDLK_RIGHT){
                pad.velocity.x = 2.0f;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                pad.velocity.x = -2.0f;
            }
        }
        if(event.type == SDL_KEYUP){
            if(event.key.keysym.sym == SDLK_RIGHT){
                pad.velocity.x = 0.0f;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                pad.velocity.x = 0.0f;
            }
        }
    }

    void update(){
        pad.update();
        ball.update();

        if(ball.rct.x < delimiter.x ||
            ball.rct.x + ball.rct.w > delimiter.x + delimiter.w){
            ball.velocity.x *= -1;
        }

        if(ball.rct.y < delimiter.y ||
            ball.rct.y + ball.rct.h > delimiter.y + delimiter.h){
            ball.velocity.y *= -1;
        }
        
        if(rct_collide(pad.rct, ball.rct)){
            ball.velocity.y *= -1;
            ball.rct.y = pad.rct.y - ball.rct.h - 1; 
        }

        for(auto& block : blocks){
            if(rct_collide(block.rct, ball.rct)){
                block.active = false;
                ball.velocity.y *= -1;
            }
        }

    }

    Pad pad;
    Ball ball;
    std::vector<Block> blocks;
    SDL_Rect delimiter;

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    BlocksGame blocksGame(70,50,400,400);
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
            blocksGame.handle_input(game.event);
        }

        blocksGame.update();
        blocksGame.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

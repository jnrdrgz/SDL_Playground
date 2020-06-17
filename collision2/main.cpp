#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

bool tCols(SDL_Rect& A, SDL_Rect& B)
{
    bool d0 = B.w + B.x < A.x;
    bool d1 = A.w + A.x < B.x;
    bool d2 = B.h + B.y < A.y;
    bool d3 = A.h + B.y < B.y;
    return !(d0 | d1 | d2 | d3);
}

bool rct_collide(SDL_Rect& a, SDL_Rect& b){
    if( a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y)
    {
        return true;
    }

    return false;
}

struct Box
{
    Box(int x, int y, int w, int h, SDL_Color c) 
    : rct{x,y,w,h},
    color{c.r,c.g,c.b,c.a}
    {}

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
        SDL_RenderFillRect(renderer, &rct);
    }

    void update(){
        rct.x += vx;
        rct.y += vy;
    }

    SDL_Rect rct; 
    SDL_Color color; 
    int vx, vy;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    SDL_Color red = {255,0,0,255};
    SDL_Color black = {0,0,0,255};
    Box box{40,40,16,16,red};

    Box platform{0+32,300,screen_w-64,16,black};    
    Box tinybox{40,80,8,8,black};
    Box stick{screen_w-32,0+32,16,16*6,black};
    Box equal{screen_w/2,platform.rct.y-16,16,16,black};    

    Box bigbox{150,125,box.rct.w*7,box.rct.h*7,black};
    
    int middle_platx = platform.rct.x+(platform.rct.w/2);        
    int middle_platy = platform.rct.y+(platform.rct.h/2);
    printf("%d,%d\n", middle_platx,middle_platy);
        
    float fps = 60.0f;
    float dt = 1.0f/fps*1000.0f;

    Uint32 start_time;

    int vel = 4;
    while(game.running){
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            printf("Pressing M\n");
        }

        box.vx = 0;
        box.vy = 0;

        if(kbstate[SDL_SCANCODE_RIGHT]){
            box.vx = vel;
            if(rct_collide(platform.rct,box.rct)){
                if(box.rct.x < middle_platx) box.vx = 0;
            }
        }
        if(kbstate[SDL_SCANCODE_LEFT]){
            box.vx = -vel;
            if(rct_collide(platform.rct,box.rct)){
                if(box.rct.x > middle_platx) box.vx = 0;
            }
        }
        if(kbstate[SDL_SCANCODE_UP]){
            box.vy = -vel;
            if(rct_collide(platform.rct,box.rct)){
                if(box.rct.y > middle_platy) box.vy = 0;
            }
        }
        if(kbstate[SDL_SCANCODE_DOWN]){
            box.vy = vel;
            if(rct_collide(platform.rct,box.rct)){
                if(box.rct.y < middle_platy) box.vy = 0;
            }
            
        }
        

        if(kbstate[SDL_SCANCODE_P]){
            printf("--------\n");
            
            //printf("BOX - PLAT\n");
            //printf("%d\n", tCols(box.rct,platform.rct));
            //printf("PLAT - BOX\n");
            //printf("%d\n", tCols(platform.rct,box.rct));
            

            printf("%d\n", (rct_collide(platform.rct,box.rct)) );
            
            printf("--------\n");
        }

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

        box.update();

        platform.draw(game.renderer);
        tinybox.draw(game.renderer);
        stick.draw(game.renderer);
        equal.draw(game.renderer);
        bigbox.draw(game.renderer);

        box.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);       
    }
    
    
    game.close();
}

#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

namespace Pong{
    struct Game
    {
        Game(int x, int y) : 
        container{x,y,400,400},
        paddle_p1{x+5, y, container.w/32, container.h/8},
        paddle_p2{container.x+container.w-paddle_p1.w-5, container.y+container.h-paddle_p1.h,
            paddle_p1.w,paddle_p1.h},
        ball{(x+container.w)/2,(y+container.h)/2,container.w/35,container.w/35},
        n_dividers{10},
        ballvx{4}, ballvy{4}, base_v{4},
        padv{6}, pad2v{6},
        points1{0},points2{0},
        container_center_x{(container.w/2)+container.x},
        container_center_y{(container.h/2)+container.y}
        {
            int diveder_w = paddle_p1.w/2;
            

            int distance_between_containers = container.h/10;
            int divider_y = container.y;

            dividers = (SDL_Rect*)malloc(sizeof(SDL_Rect)*10);

            for(int i = 0; i < n_dividers; i++){
                dividers[i] = {container_center_x,divider_y,diveder_w,paddle_p1.h/2};
                divider_y += distance_between_containers;
            }
        }

        ~Game(){
            free(dividers);
        }
        
        void handle_input(){
            const Uint8 *kbstate = SDL_GetKeyboardState(NULL);

            padv = 0;
            if(kbstate[SDL_SCANCODE_UP]){
                padv = -6;
            }
            
            if(kbstate[SDL_SCANCODE_DOWN]){
                padv = 6;
            }
        }
        
        void update(){
            //vel update
            ball.x += ballvx;
            ball.y += ballvy;
            paddle_p1.y += padv;

            //pad collision
            if(rct_collide(paddle_p1,ball)){
                ballvx = base_v;
            }
            if(rct_collide(paddle_p2,ball)){
                ballvx = -base_v;
            }

            //enemy AI
            if(ball.x > container.x+(container.w/2) && ballvx > 0){
                if(ball.y > paddle_p2.y){
                    paddle_p2.y += pad2v;
                }

                if(ball.y < paddle_p2.y){
                    paddle_p2.y -= pad2v;
                }
            }

            //check of someone lose
            if(ball.x < container.x){
                ballvx = base_v;
                ball.x = (container.x + container.w)/2;
                ball.y = (container.y + container.h)/2;
                printf("lose 1\n");
                points2++;
            }  
            
            if(ball.x + ball.w > container.x+container.w){
                ballvx = -base_v;
                ball.x = (container.x + container.w)/2;
                ball.y = (container.y + container.h)/2;
                printf("lose 2\n");
                points1++;
            }
            
            //check ball dont trepass boundaries
            if(ball.y+ball.h > container.y+container.h) ballvy = -base_v;
            if(ball.y < container.y) ballvy = base_v; 

            //check paddles dont trepass boundaries
            if(paddle_p1.y < container.y) paddle_p1.y = container.y; 
            if(paddle_p1.y+paddle_p1.h > container.y+container.h) paddle_p1.y = container.y+container.h-paddle_p1.h;

            if(paddle_p2.y < container.y) paddle_p2.y = container.y; 
            if(paddle_p2.y+paddle_p2.h > container.y+container.h) paddle_p2.y = container.y+container.h-paddle_p2.h;
        }

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRects(renderer, dividers, n_dividers);
            SDL_RenderDrawRect(renderer, &container);
            SDL_RenderFillRect(renderer, &paddle_p1);
            SDL_RenderFillRect(renderer, &paddle_p2);
            SDL_RenderFillRect(renderer, &ball);
        }

        SDL_Rect container, paddle_p1, paddle_p2, ball, divider;
        const int n_dividers;
        SDL_Rect* dividers = NULL;
        int ballvx,ballvy,base_v;
        int padv, pad2v;
        int points1, points2;
        const int container_center_x, container_center_y; 

        bool ended{false}, paused{true};

    private:
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
    };
}

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    Pong::Game pgame(120,50);
    
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            printf("Pressing M\n");
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

        pgame.handle_input();
        pgame.update();
        pgame.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);       
    }
    
    
    game.close();
}

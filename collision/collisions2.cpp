#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();

enum class Collision {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE,
};

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

bool is_vertical_col(SDL_Rect a, SDL_Rect b){
    int va = 6; //pixels to dont permit overlay
    //if(a.y + a.h + f_pixels > b.y) return true;
    //if(a.y + f_pixels < b.y + b.h) return false;
    //if(a.y < b.y+b.h && a.y > b.y
    //    && a.y+a.h > b.y) return false;

    if(a.y+a.h < b.y+va) return true;
    if(a.y > b.y+b.h-va) return true;
    return false;
}

Collision what_side_col(SDL_Rect a, SDL_Rect b){
    int va = 6; 
    
    if(rct_collide(a, b)){
        if(a.y+a.h < b.y+va) return Collision::UP;
        if(a.y > b.y+b.h-va) return Collision::DOWN;
        if(a.x+a.w < b.x+va) return Collision::LEFT;
        if(a.x > b.x+b.w-va) return Collision::RIGHT; 
    }
    
    return Collision::NONE;
}

struct Platforms
{
public:
    void draw(SDL_Renderer* renderer){
        for(auto r : rcts){
            SDL_SetRenderDrawColor(renderer, 255,255,0, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }

    void add(int x, int y){
        SDL_Rect r = {x,y,90,15};
        rcts.push_back(r); 
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_MOUSEMOTION){
            if(SDL_BUTTON(SDL_BUTTON_LEFT)){
                printf("%d,%d\n",event.button.x, event.button.y );
                SDL_Rect mouse_click = {event.button.x, event.button.y, 10, 10};
                for(auto& r : rcts){
                    if(rct_collide(mouse_click, r)){
                        r.x = mouse_click.x-(r.w/2);    
                        r.y = mouse_click.y-(r.h/2);
                    }
                }
            }

        }
    }

    void handle_input(){
        int x, y;
        if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            //SDL_Log("MOUSE LEFT PRESSED");
            //printf("%d,%d\n",event.button.x, event.button.y );
            SDL_Rect mouse_click = {x,y, 10, 10};
            for(auto& r : rcts){
                if(rct_collide(mouse_click, r)){
                    r.x = mouse_click.x-(r.w/2);    
                    r.y = mouse_click.y-(r.h/2);
                }
            }
        }

    }

    std::vector<SDL_Rect> rcts = {};

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    SDL_Rect r1 = {130,100, 15, 15};
    SDL_Rect r2 = {200,200, 90, 15};

    int velx = 6;
    int vely = 6;

    std::string collision = "not";
    std::string where = "not";

    log_system.add_text("collision", collision, game.renderer);
    log_system.add_text("where", where, game.renderer);

    Platforms plts;
    
    for(int i = 0; i < 10; i++){
        plts.add(0, 30*i);
    }

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        plts.handle_input();
        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                    r1.x += velx;
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    r1.x -= velx;
                }
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    r1.y += vely;
                }
                if(game.event.key.keysym.sym == SDLK_UP){
                    r1.y -= vely;
                }
            }
            //plts.handle_input(game.event);
        }
        
        SDL_SetRenderDrawColor( game.renderer, 255,0,0, 255);
        SDL_RenderFillRect(game.renderer, &r1);
        plts.draw(game.renderer);

        r1.x += velx;
        r1.y += vely;

        collision = "not";
        for(auto r : plts.rcts){
            if(rct_collide(r1, r)){
                collision = "colliding";
            }

            switch(what_side_col(r1, r)){
                case Collision::UP:
                    vely = -6;
                    where = "up";
                break;
                case Collision::DOWN:
                    vely = 6;
                    where = "down";
                break;
                case Collision::LEFT:
                    velx = -6;
                    where = "left";
                break;
                case Collision::RIGHT:
                    velx = 6;
                    where = "right";
                break;
                case Collision::NONE:

                break;
                
            }
        }
                
        if(r1.x>500 || r1.x<0) velx *= -1;
        if(r1.y>500 || r1.y<0) vely *= -1;

        log_system.update_text("collision", collision, game.renderer);
        log_system.update_text("where", where, game.renderer);
        log_system.draw(game.renderer);

        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}

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
    plts.add(0, 100);
    plts.add(200, 200);
    plts.add(100, 100);
    plts.add(0, 200);

    plts.add(100, 300);
    plts.add(300, 100);
    plts.add(300, 300);

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
        }
        
        SDL_SetRenderDrawColor( game.renderer, 255,255,0, 255);
        SDL_RenderFillRect(game.renderer, &r2);    
        
        SDL_SetRenderDrawColor( game.renderer, 255,0,0, 255);
        SDL_RenderFillRect(game.renderer, &r1);
        plts.draw(game.renderer);

        r1.x += velx;
        r1.y += vely;

        for(auto r : plts.rcts){
            switch(what_side_col(r1, r)){
                case Collision::UP:
                    vely = -6;
                break;
                case Collision::DOWN:
                    vely = 6;
                break;
                case Collision::LEFT:
                    velx = -6;
                break;
                case Collision::RIGHT:
                    velx = 6;
                break;
                case Collision::NONE:
                break;
                
            }
        }


        /*if(rct_collide(r1, r2)){
            collision = "colliding";
            if(is_vertical_col(r1, r2)){
                //printf("vertical colliding\n");
                where = "vertical";
                //vely*=-1;
                //r1.y += vely;
            } else {
                //printf("horizontal colliding\n");
                where = "horizontal";
                //velx*=-1;
                //r1.x += velx;
            }
        } else {
            collision = "not";
            where = "not";
        }*/

        

        if(rct_collide(r1, r2)){
            collision = "colliding";
        } else {
            collision = "not";
            where = "not";
        }
        
        switch(what_side_col(r1, r2)){
            case Collision::UP:
                where = "UP";
            break;
            case Collision::DOWN:
                where = "DOWN";
            break;
            case Collision::LEFT:
                where = "LEFT";
            break;
            case Collision::RIGHT:
                where = "RIGHT";
            break;
            case Collision::NONE:
                where = "NONE";
            break;
            
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

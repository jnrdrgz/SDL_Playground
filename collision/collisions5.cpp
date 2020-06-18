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

unsigned char side_col_b(){
/*0000
UPLR

LEFT,
RIGHT,
UP,
DOWN,*/
    unsigned char col = 0x0;
    return col;
}


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

uint8_t side_col_b(SDL_Rect a, SDL_Rect b, int vax, int vay){ 
    int _vay = vay < 0 ? vay*-1 : vay;
    int _vax = vax < 0 ? vax*-1 : vax;
    uint8_t col = 0x0;

    if(rct_collide(a, b)){
        if(a.y+a.h < b.y+_vay+1) col |= 0x8;//Collision::UP;
        if(a.y > b.y+b.h-_vay-1) col |= 0x4;//Collision::DOWN;
        if(a.x+a.w < b.x+_vax+1) col |= 0x2;//Collision::LEFT;
        if(a.x > b.x+b.w-_vax-1) col |= 0x1;//Collision::RIGHT; 
    }
    
    return col;
}


Collision what_side_col(SDL_Rect a, SDL_Rect b, int vax, int vay){
    //int va = 6; 
    
    int _vay = vay < 0 ? vay*-1 : vay;
    int _vax = vax < 0 ? vax*-1 : vax;

    if(rct_collide(a, b)){
        if(a.y+a.h < b.y+_vay+1) return Collision::UP;
        if(a.y > b.y+b.h-_vay-1) return Collision::DOWN;
        if(a.x+a.w < b.x+_vax+1) return Collision::LEFT;
        if(a.x > b.x+b.w-_vax-1) return Collision::RIGHT; 
    }
    
    return Collision::NONE;
}

Collision side(SDL_Rect a, SDL_Rect b, int vax, int vay){
    printf("%d,%d\n",a.y+a.h,b.y);

    int _vay = vay < 0 ? vay*-1 : vay;
    int _vax = vax < 0 ? vax*-1 : vax; 
    if(a.y+a.h < b.y+_vay) return Collision::UP;
    if(a.y > b.y+b.h-_vay) return Collision::DOWN;
    if(a.x+a.w < b.x+_vax) return Collision::LEFT;
    if(a.x > b.x+b.w-_vax) return Collision::RIGHT; 
    return Collision::NONE;
}



struct RigidBody
{
    RigidBody(){
        rct = {130,100, 15, 15};
        back = {130,100, 15, 15};
    }

    void draw(SDL_Renderer* renderer){
        
        SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
        
        SDL_RenderFillRect(renderer, &rct);
    }

    void update(){
        lx = rct.x;
        ly = rct.y;
        //rct.x += vx;
        //rct.y += vy;
    }

    void handle_input(SDL_Event event){
        if(event.key.keysym.sym == SDLK_RIGHT){
            rct.x += vx;
        }
        if(event.key.keysym.sym == SDLK_LEFT){
            rct.x -= vx;
        }
        if(event.key.keysym.sym == SDLK_DOWN){
           rct.y += vy;
        }
        if(event.key.keysym.sym == SDLK_UP){
           rct.y -= vy;
        }
    }

    SDL_Rect rct,back;
    int lx, ly;
    int vx, vy;
};

Collision rb_rct_collision(RigidBody rb, SDL_Rect b){
    SDL_Rect a = {rb.lx,rb.ly,rb.rct.w,rb.rct.h};
    if(rct_collide(rb.rct, b)){
        printf("%d, %d  \n", a.y+a.h,b.y);
        if(a.y+a.h < b.y) return Collision::UP;
        if(a.y > b.y+b.h) return Collision::DOWN;
        if(a.x+a.w < b.x) return Collision::LEFT;
        if(a.x > b.x+b.w) return Collision::RIGHT; 
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
        SDL_Rect r = {x,y,90,25};
        rcts.push_back(r); 
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_MOUSEMOTION){
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

    int ly = 0, lx = 0;

    int velx = 6;
    int vely = 6;

    std::string collision = "not";
    std::string where = "not";

    Platforms plts;

    int spacing = 75;
    for(int i = 0; i < 8; i++){
        plts.add(random()%200, 75*i);
        plts.add((random()%200)+200, 75*i);
    }

    log_system.add_text("collision", collision, game.renderer);
    log_system.add_text("where", where, game.renderer);

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
                    lx = r1.x;
                    ly = r1.y;
                    r1.x += velx;
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    lx = r1.x;
                    ly = r1.y;
                    r1.x -= velx;
                }
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    lx = r1.x;
                    ly = r1.y;
                    r1.y += vely;
                }
                if(game.event.key.keysym.sym == SDLK_UP){
                    lx = r1.x;
                    ly = r1.y;
                    r1.y -= vely;
                }

            }

            if(game.event.type == SDL_MOUSEBUTTONDOWN){
                printf("%d,%d\n",game.event.button.x, game.event.button.y );
                r1.x = game.event.button.x;
                r1.y = game.event.button.y;
                //velx = 6;
                //vely = 6;
            }
            //plts.handle_input(game.event);
            //rb.handle_input(game.event);
        }

        lx = r1.x;
        ly = r1.y;            
        r1.x += velx;
        r1.y += vely;
        collision = "not";
        Collision col = Collision::NONE;

        SDL_Rect r_ex = {lx,ly,r1.w,r1.h};


        for(auto r : plts.rcts){
            if(rct_collide(r1, r)){
                collision = "colliding";
                col = side(r_ex,r,velx,vely);
            }
            if(col == Collision::UP){
                //rb.vy = -vely;
                
                vely = -6;

                where = "up";
            }
            if(col == Collision::DOWN){
                //rb.vy = vely;
                vely = 6;
                where = "down";
            }
            if(col == Collision::RIGHT){
                //rb.vx = velx;
                velx = 6;
                where = "right";
            }
            if(col == Collision::LEFT){
                //rb.vx = -velx;
                velx = -6;
                where = "left";
            }
            if(col == Collision::NONE){}
        }

        //SDL_SetRenderDrawColor(game.renderer, 255,255,0, 255);
        //SDL_RenderFillRect(game.renderer, &r2);
        plts.draw(game.renderer);
        SDL_SetRenderDrawColor(game.renderer, 255,0,0, 255);
        SDL_RenderFillRect(game.renderer, &r1);
        //SDL_SetRenderDrawColor(game.renderer, 0,0,0, 255);
        //SDL_RenderDrawRect(game.renderer, &r_ex);
        
                
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

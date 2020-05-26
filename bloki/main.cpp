#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"
#include <vector>
#include <algorithm>

static LogSystem log_system = LogSystem();

SDL_Color get_random_rainbow_color(){
    //Uint8 numbers[3] = {255,127,0};
    std::vector<Uint8> numbers = {255,127,0};
 
    std::random_shuffle(numbers.begin(), numbers.end());

    Uint8 r = numbers[0];
    Uint8 g = numbers[1];
    Uint8 b = numbers[2];
    /*if(r == 0 && g == 0 && b == 0){
        r = numbers[random()%2];
        g = numbers[random()%2];
        b = numbers[random()%2];
    }*/

    SDL_Color color = {r,g,b,255};

    return color;
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
    int va = 2; //pixels to dont permit overlay

    if(a.y+a.h < b.y+va) return true;
    if(a.y > b.y+b.h-va) return true;
    return false;
}
enum class Collision {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE,
};

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

        this->color = color;
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

    void set_dimensions(int w, int h){
        rct.w = w;
        rct.h = h;

        back.w = w;
        back.h = h;
    }

    void update(){
        back.x = rct.x;
        back.y = rct.y;

        rct.x += velocity.x;
        rct.y += velocity.y;
    }

    SDL_Rect rct;
    SDL_Rect back;
    Vector2 velocity;

};


struct BlocksGame
{
public:
    BlocksGame(int x, int y, int w, int h){
        // for restart wo parameters
        this->x = x;
        this->y = y;
        this->h = h;
        this->w = w;

        start(x,y,w,h);
    }

    void start(int x, int y, int w, int h){
        delimiter.x = x;
        delimiter.y = y;
        delimiter.w = w;
        delimiter.h = h;

        int bx = delimiter.x + 20;
        int by = delimiter.y + 40;
        int bw = delimiter.w / 20;
        int bh = delimiter.h / 40;

        for(int i = 0; i < 100; i++){
            SDL_Color color = get_random_rainbow_color();
            blocks.push_back(Block(bx,by,bw,bh,color));
            bx += bw;// + 5;
            if(bx > delimiter.x+delimiter.w-bw-bw){
                bx = delimiter.x + 20;
                by += bh;//+(ball_w-5);
            }
        }

        pad.rct.w = bw*2;
        pad.rct.h = bh;//delimiter.h/25;
        pad.rct.x = (delimiter.x + delimiter.x/2);
        pad.rct.y = (delimiter.y+delimiter.h) - pad.rct.h;

        int ball_w = delimiter.w/42;
        int ball_h = ball_w;
        ball.set_dimensions(ball_w,ball_h);
        ball.rct.x = pad.rct.x;
        ball.rct.y = pad.rct.y - ball.rct.h;
        printf("ball size: %d\n", ball_w);
        
        pad_vel = 7.0f;
        ball_velx = 5.0f;
        ball_vely = 5.0f;
    }

    void restart(){
        blocks.clear();
        start(x, y, w, h);
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
                ball.velocity.x = ball_velx;
                ball.velocity.y = -ball_vely;
            }
            if(event.key.keysym.sym == SDLK_t){
                ball.velocity.x *= -1;
                ball.velocity.y *= -1;
            }
            if(event.key.keysym.sym == SDLK_r){
                restart();
            }
            if(event.key.keysym.sym == SDLK_RIGHT){
                pad.velocity.x = pad_vel;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                pad.velocity.x = -pad_vel;
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

            //test cleaner collsion
            if(ball.rct.x + ball.rct.w > delimiter.x + delimiter.w){
                ball.rct.x = delimiter.x + delimiter.w - ball.rct.w;
            }
            if(ball.rct.x < delimiter.x){
                ball.rct.x = delimiter.x;
            }
        }

        if(ball.rct.y < delimiter.y ||
            ball.rct.y + ball.rct.h > delimiter.y + delimiter.h){
            ball.velocity.y *= -1;

            //test cleaner collsion
            if(ball.rct.y + ball.rct.h > delimiter.y + delimiter.h){
                ball.rct.y = delimiter.y + delimiter.h - ball.rct.h;
            }
            if(ball.rct.y < delimiter.y){
                ball.rct.y = delimiter.y;
            }
        }

        if(rct_collide(pad.rct, ball.rct)){
            ball.velocity.y *= -1;
            ball.rct.y = pad.rct.y - ball.rct.h - 1; 
            
            if(ball.rct.x-pad.rct.x > pad.rct.w/2){
                if(ball.velocity.x < 0) ball.velocity.x *= -1;
            } else {
                if(ball.velocity.x > 0) ball.velocity.x *= -1;
            }
        }

        bool already_destroyed_a_block = false;
        for(auto& block : blocks){
            if(rct_collide(block.rct, ball.rct) && block.active){
                block.active = false;
                if(!already_destroyed_a_block){
                    /*if(is_vertical_col(ball.rct, block.rct)){
                        ball.velocity.y *= -1;
                    } else {
                        ball.velocity.x *= -1;
                        printf("not vertical col\n");
                    }*/

                    //Collision col = what_side_col(ball.rct, block.rct, (int)ball_velx,(int)ball_vely);
                    Collision col = side(ball.back, block.rct, (int)ball_velx,(int)ball_vely);
                    
                    //v
                    if(col == Collision::UP){
                        ball.velocity.y = -5.0f;
                    }
                    if(col == Collision::DOWN){
                        ball.velocity.y = 5.0f;
                    }
                    if(col == Collision::LEFT){
                        ball.velocity.x = -5.0f;
                    }
                    if(col == Collision::RIGHT){
                        ball.velocity.x = 5.0f;
                    }
                    if(col == Collision::NONE){ball.velocity.x *=-1;ball.velocity.y *=-1;}//?? neve leave a collision wp resolve

                    ball.update();

                    already_destroyed_a_block = true;

                }
            }
        }

    }

    Pad pad;
    Ball ball;
    std::vector<Block> blocks;
    SDL_Rect delimiter;
    float pad_vel, ball_velx, ball_vely;
    int x,y,h,w;

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

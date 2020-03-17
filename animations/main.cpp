#include <SDL2/SDL.h>
#include "../timer.h"
#include "../log_system/log_system.h"
#include "game.h"
#include <vector>
#include <algorithm> 
#include <string>

static LogSystem log_system = LogSystem();

enum BombAnimationState
{
    STEADY,
    ABOUT_TO_EXPLODE,
    EXPLODING,    
};

bool clicked = false;

class Sprite
{
private:
    SDL_Texture* texture = nullptr;
    std::vector<SDL_Rect> src_rct_v;
    SDL_Rect current_src;
    int current_frame;
    SDL_Rect dst_rct = {0,0,100,100};
    
public:
    Sprite(){

    }
    Sprite(SDL_Renderer *r, std::string file_name, int src_rows, int src_cols,
        int src_width, int src_height){
        
        SDL_Surface* tmp_srf = IMG_Load(file_name.c_str());
        if(!tmp_srf){
            printf("Error loading surface: \n, %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        if(!texture){
            printf("Error loading texture: \n, %s\n", SDL_GetError());
        }

        SDL_FreeSurface(tmp_srf);

        int x = 0;
        int y = 0;
        int w = src_width;
        int h = src_height;
        for(int i = 0; i<src_rows; i++){
            for(int j = 0; j<src_cols; j++){
                SDL_Rect r = {x,y,h,w};
                src_rct_v.push_back(r);
                x+=w;
            }
            y+=h;
            x = 0;
        }

        for(auto rct : src_rct_v){
            printf("%d-%d\n", rct.x,rct.y);
        }

        current_frame = 0;
        current_src = src_rct_v[current_frame];
    }

    void set_x(int x){
        dst_rct.x = x;
    }

    void set_y(int y){
        dst_rct.y = y;
    }

    void draw(SDL_Renderer* r){

        SDL_RenderCopyEx(r, texture, &current_src, &dst_rct, 0, 0, SDL_FLIP_NONE);
    }

    bool next_frame(){
        if(current_frame < src_rct_v.size()){
            current_frame++;
            current_src = src_rct_v[current_frame];
            return true;
        } else{
            current_frame = 0;
            current_src = src_rct_v[0];
            return false;
        }
    }

};

class Explosion
{
private:
    Sprite sprite;
    int qFrames = 4*4;
    bool readytoanimate = false;
    bool clicked = false;

public:
    Explosion(SDL_Renderer *r){
        sprite = Sprite(r, "explosion_r2.png", 4, 4,
        64, 64);

        sprite.set_x(50);
        sprite.set_y(50);
    }

    Explosion(SDL_Renderer *r, int x, int y){
        sprite = Sprite(r, "explosion_r2.png", 4, 4,
        64, 64);

        sprite.set_x(x);
        sprite.set_y(y);
    }

    void draw(SDL_Renderer* r, int frame, int avgFrames){
        sprite.draw(r);

        std::vector<int> in_what_frame_animate;
        for(int i = avgFrames/(qFrames); avgFrames + 1 > i; i += avgFrames/qFrames){
            in_what_frame_animate.push_back(i);
        }

        
        if(std::find(in_what_frame_animate.begin(), in_what_frame_animate.end(), frame) != in_what_frame_animate.end()){
            if(!sprite.next_frame()){
                readytoanimate = false;
            }
        }
    
    }

    void handle_input(SDL_Event e){
        

    }

    void update(){

    }

};

class ExplosionCreator
{
private:
    bool clicked;
    std::vector<Explosion*> explosions;
public:
    ExplosionCreator(){

    }

    void handle_input(SDL_Event e, SDL_Renderer* r){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                this->clicked = true; 
                explosions.push_back(new Explosion(r, e.button.x, e.button.y));   
            }

        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && this->clicked){
                this->clicked = false;
            }
        }
    }

    void draw(SDL_Renderer* r, int frame, int avgFrames){
        for(auto explosion : explosions){
            explosion->draw(r, frame, avgFrames);
        }
    }


};

class Bomb
{
public:
    SDL_Texture* texture = nullptr;
    //SDL_Rect src_rct = {0,0,200,200};
    std::vector<SDL_Rect> src_rct_v;    
    SDL_Rect dst_rct = {200,200,200,200};
    BombAnimationState state = STEADY;
    int animationFrame = 0;
    int qFrames = 4;

    Bomb(SDL_Renderer *r){
        SDL_Surface* tmp_srf = IMG_Load("dynamite_corrected.png");
        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        SDL_FreeSurface(tmp_srf);

        int x = 0;
        int y = 0;
        int w = 200;
        int h = 200;
        for(int i = 0; i<5; i++){
            SDL_Rect r = {x,y,h,w};
            src_rct_v.push_back(r);
            x+=w;
        }
    }



    void draw(SDL_Renderer* r, int frame, int avgFrames){
        if(state == STEADY){
            SDL_RenderCopyEx(r, texture, &src_rct_v[0], &dst_rct, 0, 0, SDL_FLIP_NONE);
            
        } else if (state == ABOUT_TO_EXPLODE){
            SDL_RenderCopyEx(r, texture, &src_rct_v[animationFrame], &dst_rct, 0, 0, SDL_FLIP_NONE);
           // for(int i = avgFrames/qFrames; avgFrames + 1 < i; i += avgFrames/qFrames){

            //}

            std::vector<int> in_what_frame_animate;
            for(int i = avgFrames/qFrames; avgFrames + 1 > i; i += avgFrames/qFrames){
                in_what_frame_animate.push_back(i);
            }

            //if(frame == 12 || frame == 24 || frame == 36 || frame == 48 || frame == 60){
            //  animationFrame++;
            //}
            if(std::find(in_what_frame_animate.begin(), in_what_frame_animate.end(), frame) != in_what_frame_animate.end()){
                animationFrame++;
            }

            if(animationFrame >= src_rct_v.size()){
                animationFrame = 0;
                state = STEADY;
            }
        } else if (state == EXPLODING){

        }
    }

    void handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !clicked){
                clicked = true;
                state = ABOUT_TO_EXPLODE;
            }

        } 
        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && clicked){
                
                clicked = false;
            }            
        }

    }

    void update(){

    }

};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    
    Bomb bomb(game.renderer);
    Explosion explosion(game.renderer);
    Timer timer;
    timer.start();

    int frame = 0;
    int animationFrame = 0;


    bool log = true;
    log_system.add_text("AVG_FPS", "", game.renderer);
    log_system.add_text("AVG_DT", "", game.renderer);
    log_system.add_text("DT", "", game.renderer);
    log_system.add_text("FPS", "", game.renderer);

    float avgDt = 0;

    Uint32 start_time_frame = 0;
    Uint32 finish_time_frame = 0;
    Uint32 dt = 0;

    int fps = 0;

    while(game.running){
        start_time_frame = SDL_GetTicks();
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
    
            }

            bomb.handle_input(game.event);
            explosion.handle_input(game.event);
        }


        float avgFPS = (float)frame / (float)timer.getTicks_in_seconds();
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

        bomb.draw(game.renderer, animationFrame, (int)avgFPS);
        explosion.draw(game.renderer, animationFrame, (int)avgFPS);

        if(avgFPS != 0){
            avgDt = 1.0/(float)avgFPS;
        }


        if(log){
            log_system.update_text("AVG_FPS", std::to_string(avgFPS), game.renderer);
            log_system.update_text("AVG_DT", std::to_string(avgDt), game.renderer);
            log_system.update_text("DT", std::to_string(dt), game.renderer);
            log_system.update_text("FPS", std::to_string(fps), game.renderer);
            
            log_system.draw(game.renderer);
        }
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);


        dt = (SDL_GetTicks() - start_time_frame);
        fps = (int)((float)1000/(float)dt);

        frame++;
        animationFrame++;
        //printf("%d\n", animationFrame);
        if(animationFrame > (int)avgFPS){
            animationFrame = 0;
        }

        //SDL_Delay(dt);

    }
    
    game.close();
}

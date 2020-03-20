#include <SDL2/SDL.h>
#include "../timer.h"
#include "../log_system/log_system.h"
#include "game.h"
#include <vector>
#include <algorithm> 
#include <string>

static LogSystem log_system = LogSystem();

class AnimationTimer
{
private:
    int time;
    int animation_time;
    bool clicked;
    bool stopped;

public:
    AnimationTimer(){
        time = 0;
        animation_time = 1000;
        clicked = false;
        stopped = false;
    }

    AnimationTimer(int animation_time){
        time = 0;
        this->animation_time = animation_time;
        clicked = false;
        stopped = false;
    }

    void add_time(Uint32 dt){
        if(!stopped){
            time += dt;
            if(time > animation_time){
                reset();
            }
        }
    }

    int get_time(){
        return time;
    }

    int get_limit(){
        return animation_time;
    }

    void set_limit(int limit){
        animation_time = limit;
    }

    void reset(){
        time = 0;
    }

    void stop(){
        stopped = true;
    }

    void run(){
        stopped = false;
    }

    bool isRunning(){
        return !stopped;
    }

    void handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                stopped = !stopped;
                printf("click down\n");
                clicked = true;    
            }
        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && this->clicked){
                printf("click up\n");
                clicked = false;    
            }
        }
    }
};

class AnimatedTimeSprite
{
private:
    AnimationTimer anim_timer;
    SDL_Texture* texture = nullptr;
    
    int textureFrames_columns = 4; //
    int textureFrames_rows = 4; //
    int totalTextureFrames = textureFrames_columns*textureFrames_rows;
    SDL_Rect src_rct;
    SDL_Rect dst_rct = {100,100,200,200};
    int frame_time_jump;
    
    bool paused = false;
    bool clicked = false;

    void load_texture(SDL_Renderer* r, std::string file_name){
        SDL_Surface* tmp_srf = IMG_Load(file_name.c_str());
        if(!tmp_srf){
            printf("Error loading surface: \n, %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        if(!texture){
            printf("Error loading texture: \n, %s\n", SDL_GetError());
        }

        SDL_FreeSurface(tmp_srf);
    }
public:
    AnimatedTimeSprite(SDL_Renderer* r, std::string file_name, int tfc, int tfr, int w, int h, int animation_time){
        textureFrames_columns = tfc;
        textureFrames_rows = tfr;
        totalTextureFrames = textureFrames_columns*textureFrames_rows;

        dst_rct.w = w;
        dst_rct.h = h;

        src_rct.x = 0;
        src_rct.y = 0;
        src_rct.w = w;
        src_rct.h = h;

        anim_timer.set_limit(animation_time);
        frame_time_jump = (int)((float)anim_timer.get_limit()/(float)totalTextureFrames);

        load_texture(r, file_name);
        
    }

    AnimatedTimeSprite(){
        dst_rct.x = 100;
        dst_rct.y = 100;
        
        anim_timer.set_limit(2000);   
        frame_time_jump = (int)((float)anim_timer.get_limit()/(float)totalTextureFrames);
    }

    ~AnimatedTimeSprite(){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    void update(int dt){
        anim_timer.add_time(dt);
        src_rct.x = ((int)((float)anim_timer.get_time()/(float)frame_time_jump)) * src_rct.w;
    }

    int TESTING_get_src_x(){
        return src_rct.x;
    }

    void TESTING_set_position(int x, int y){
        dst_rct.x = x;
        dst_rct.y = y;
    }

    void TESTING_handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                if(anim_timer.isRunning()){
                    anim_timer.stop();
                    anim_timer.reset();
                } else {
                    anim_timer.run();
                }

                printf("horse anim timer stopped\n");
                this->clicked = true;
            }

        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && this->clicked){
                printf("horse anim click up\n");
                this->clicked = false;

            }
        }
    }

    void draw(SDL_Renderer* r){
        SDL_RenderCopyEx(r, texture, &src_rct, &dst_rct, 0, 0, SDL_FLIP_NONE);
    }

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Testing", "HELLO", NULL);

    
    Timer timer;
    timer.start();
    AnimationTimer animTimer(5*1000);
    //AnimatedTimeSprite my_animated_sprite(game.renderer, "dynamite_corrected.png", 5, 1, 200, 200, 1*1000);

    AnimatedTimeSprite horse(game.renderer, "horse.png", 19, 1, 100, 100, 1*2000);
    AnimatedTimeSprite horse1(game.renderer, "horse.png", 19, 1, 100, 100, 1*1000);
    AnimatedTimeSprite horse2(game.renderer, "horse.png", 19, 1, 100, 100, 1*500);

    horse.TESTING_set_position(240, 140);
    horse1.TESTING_set_position(240, 250);
    horse2.TESTING_set_position(240, 360);

    int frame = 0;
    int animationFrame = 0;

    bool log = true;
    log_system.add_text("AVG_FPS", "", game.renderer);
    log_system.add_text("AVG_DT", "", game.renderer);
    log_system.add_text("DT", "", game.renderer);
    log_system.add_text("FPS", "", game.renderer);
    log_system.add_text("ANIM_SPR_TIME", "", game.renderer);
    log_system.add_text("SPRITE_X", "", game.renderer);

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

            animTimer.handle_input(game.event);
            horse.TESTING_handle_input(game.event);
        
        }


        float avgFPS = (float)frame / (float)timer.getTicks_in_seconds();
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

        
        if(avgFPS != 0){
            avgDt = 1.0/(float)avgFPS;
        }

        animTimer.add_time(dt);
        //my_animated_sprite.update(dt);
        //my_animated_sprite.draw(game.renderer);

        horse.update(dt);
        horse.draw(game.renderer);
        horse1.update(dt);
        horse1.draw(game.renderer);
        horse2.update(dt);
        horse2.draw(game.renderer);

        if(log){
            log_system.update_text("AVG_FPS", std::to_string(avgFPS), game.renderer);
            log_system.update_text("AVG_DT", std::to_string(avgDt), game.renderer);
            log_system.update_text("DT", std::to_string(dt), game.renderer);
            log_system.update_text("FPS", std::to_string(fps), game.renderer);
            log_system.update_text("ANIM_SPR_TIME", std::to_string(animTimer.get_time()), game.renderer);
            //log_system.update_text("SPRITE_X", std::to_string(my_animated_sprite.TESTING_get_src_x()), game.renderer);


            log_system.draw(game.renderer);
        }

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);


        dt = (SDL_GetTicks() - start_time_frame);
        fps = (int)((float)1000/(float)dt);

        frame++;
        animationFrame++;

        if(animationFrame > (int)avgFPS){
            animationFrame = 0;
        }

    }
    
    game.close();
}

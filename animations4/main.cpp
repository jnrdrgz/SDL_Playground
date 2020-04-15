#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

//CONSIDERATIONS:
//One game object has many animations
//One game object has an steady image
//all in the same texture
//if some variable is true it has too reproduce an animation

struct AnimationImage
{
public:
    void load_texture(SDL_Renderer* renderer, std::string file_name){
        SDL_Surface* tmp_srf = IMG_Load(file_name.c_str());
        if(!tmp_srf){
            printf("Error loading surface: \n, %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        if(!texture){
            printf("Error loading texture: \n, %s\n", SDL_GetError());
        }

        SDL_FreeSurface(tmp_srf);
    }
public:
    AnimationImage(){

    }
    AnimationImage(SDL_Renderer* r, std::string file_name, int init_f, int w, int h){
                        //int total_frames, int tfc, int w, int h){

        dst.x = 100;
        dst.y = 100;
        dst.w = w*3;
        dst.h = h*4;

        src.x = 0;
        src.y = 0;
        src.w = w;
        src.h = h;


        //textureFrames_columns = tfc;
        //totalTextureFrames = total_frames;
        initial_frame = init_f;

        load_texture(r, file_name);

    }

    void update(int frame){
        src.x = src.w*(frame + initial_frame);
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, 0, SDL_FLIP_NONE);
    }

    /*AnimatedTimeSprite(){
        dst_rct.x = 100;
        dst_rct.y = 100;
        
        anim_timer.set_limit(2000);   
        frame_time_jump = anim_timer.get_limit()/totalTextureFrames;

    }*/
    
public:
    SDL_Rect dst, src;
    SDL_Texture* texture = nullptr;
    int textureFrames_columns;
    int totalTextureFrames;
    int initial_frame;

};

struct Animation
{
public:
    Animation(){
        timer.start();
        stepTimer.start();
        frame = 1;
        totalFrames = 6;
        duration = 1000;
        step = duration/totalFrames;

        printf("step: %d\n", step);
    };

    Animation(SDL_Renderer* renderer) : Animation(){
        animationImage = AnimationImage{renderer, "maleBase/full/advnt_full.png", 1, 32, 64};
    }

    void set_duration(Uint32 duration){
        this->duration = duration;
    }

    void update(){
        if(stepTimer.getTicks() >= step){
            frame++;
            if(frame == totalFrames){
                frame = 1;
            }

            stepTimer.stop();
            stepTimer.start();
            steps_acomp++;
            //printf("step\n");
        }

        if(timer.getTicks() >= duration && steps_acomp == totalFrames){
            timer.stop();
            timer.start();

            printf("%d\n",steps_acomp );
            steps_acomp = 0;
            printf("restarted\n");
        }

        animationImage.update(frame-1);
    }

    void stop(){

    }

    void draw(SDL_Renderer* renderer){
        animationImage.draw(renderer);
    }

private:
    Timer timer;
    Timer stepTimer;
    Uint32 duration;
    int frame, totalFrames, step;
    int steps_acomp = 0;
    AnimationImage animationImage;
};

struct GameObject
{
public:
    GameObject(){
        //bool repite = true;
        Uint32 duration = 4;//seconds
        animation.set_duration(duration);
        //sprite.add("walking", repite)
    }
public:
    Animation animation;

};
int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    Animation animation(game.renderer);
    //animation.set_duration(1000);

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

        animation.update();
        animation.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

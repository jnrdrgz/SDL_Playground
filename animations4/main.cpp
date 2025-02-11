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
        dst.w = w*2;
        dst.h = h*2;

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

struct SteadyImage
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
    SteadyImage(){

    }
    SteadyImage(SDL_Renderer* r){ //std::string file_name, int init_f, int w, int h){
                        //int total_frames, int tfc, int w, int h){

        dst.x = 100;
        dst.y = 100;
        dst.w = 32*2;
        dst.h = 64*2;

        src.x = 0;
        src.y = 0;
        src.w = 32;
        src.h = 64;

        load_texture(r, "maleBase/full/advnt_full.png");

    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, 0, SDL_FLIP_NONE);
    }

public:
    SDL_Rect dst, src;
    SDL_Texture* texture = nullptr;
  

};

struct Animation
{
public:
    Animation(){
        timer.start();
        stepTimer.start();
        frame = 1;
        totalFrames = 6;
        duration = 600;
        step = duration/totalFrames;

        printf("step: %d\n", step);
    };

    Animation(SDL_Renderer* renderer) : Animation(){
        animationImage = AnimationImage{renderer, "maleBase/full/advnt_full.png", 1, 32, 64};
    }

    Animation(SDL_Renderer* renderer, std::string name, int i_frame, int w, int h) : Animation(){
        animationImage = AnimationImage{renderer, name, i_frame, w, h};
    }

    void set_duration(Uint32 duration){
        this->duration = duration;
    }

    void set_total_frames(int frames){
        totalFrames = frames;
    }

    void update(){
        if(stepTimer.getTicks() >= step){
            frame++;
            if(frame == totalFrames){
                finished = true;
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
            finished = false;
            printf("restarted\n");
        }

        animationImage.update(frame-1);
    }

    void stop(){
         stepTimer.stop();
         timer.stop();
    }

    void restart(){
         stepTimer.start();
         timer.start();
         steps_acomp = 0;
         finished = false;
    }

    void draw(SDL_Renderer* renderer){
        animationImage.draw(renderer);
    }

    bool finished = false;
private:
    Timer timer;
    Timer stepTimer;
    Uint32 duration;
    int frame, totalFrames, step;
    int steps_acomp = 0;
    AnimationImage animationImage;

};


struct Sprite
{


};

struct GameObject
{
public:
    GameObject(){
        //bool repite = true;
        //sprite.add("walking", repite)

    }
    GameObject(SDL_Renderer* renderer){
        //bool repite = true;
        //sprite.add("walking", repite)

        steadyImage = SteadyImage(renderer);
        animations["walking"] = Animation(renderer);
        animations["attacking"] = Animation(renderer, "maleBase/full/attacking.png", 0, 32, 64);
        animations["attacking"].set_total_frames(4);
    }

    void update(){
        if(!steady){
            animations["walking"].update();
        }
        if(attacking){
            animations["attacking"].update();
            if(animations["attacking"].finished){
                attacking = false;
                steady = true;
                animations["attacking"].stop();
            }
        }
    }

    void draw(SDL_Renderer* renderer){
        if(walking){
            animations["walking"].draw(renderer);
        } 
        if(steady){
            steadyImage.draw(renderer);
        }
        if(attacking){
            animations["attacking"].draw(renderer);
        }
    }

    void hande_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
                walking = true;
                steady = false;
            }

            if(event.key.keysym.sym == SDLK_a){
                attacking = true;
                animations["attacking"].restart();
                steady = false;
                walking = false;
            }
        }

        if(event.type == SDL_KEYUP){
            if(event.key.keysym.sym == SDLK_RIGHT){
                walking = false;
                steady = true;
            }
        }
    }
public:
    std::unordered_map<std::string, Animation> animations;
    SteadyImage steadyImage;
    bool walking = false;
    bool steady = true;
    bool attacking = false;

};
int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    //Animation animation(game.renderer);
    GameObject animation(game.renderer);
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
            animation.hande_input(game.event);
        }

        animation.update();
        animation.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

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

struct TextureCache
{
    TextureCache(){}

    static SDL_Texture* get_texture(SDL_Renderer* renderer, std::string name){
        auto t = textures.find(name);

        if(t == textures.end()){        
            SDL_Surface* tmp_srf = IMG_Load(name.c_str());
            if(!tmp_srf){
                printf("Error loading surface: \n, %s\n", SDL_GetError());
                throw "error surface loading";
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmp_srf);
            if(!texture){
                printf("Error loading texture: \n, %s\n", SDL_GetError());
                throw "Error loading texture";
            }

            SDL_FreeSurface(tmp_srf);

            textures[name] = texture;
            return texture;
        }

        return textures[name];
    }

private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};

std::unordered_map<std::string, SDL_Texture*> TextureCache::textures;

struct AnimationImage
{
public:
    void load_texture(SDL_Renderer* renderer, std::string file_name){
        texture = TextureCache::get_texture(renderer,file_name);
    }
public:
    AnimationImage(){

    }
    AnimationImage(SDL_Renderer* r, std::string file_name, int init_f, int w, int h){
        row = 0;

        src.x = 0;
        src.y = 0;
        src.w = w;
        src.h = h;


        //textureFrames_columns = tfc;
        //totalTextureFrames = total_frames;
        initial_frame = init_f;

        load_texture(r, file_name);

    }

    AnimationImage(SDL_Renderer* r, std::string file_name, int init_f, int row, int w, int h) : AnimationImage(r, file_name, init_f, w, h){
        this->row = row;
    }

    void update(int frame){
        src.x = src.w*frame;
        src.y = src.h*row;
    }

    void draw(SDL_Renderer* renderer, SDL_Rect* dst){
        SDL_RenderCopyEx(renderer, texture, &src, dst, 0, 0, SDL_FLIP_NONE);
    }

public:
    SDL_Rect src;
    SDL_Texture* texture = nullptr;
    int textureFrames_columns;
    int totalTextureFrames;
    int initial_frame, row;
};

struct Animation
{
public:
    Animation(){
        timer.start();
        stepTimer.start();
        frame = 0;
        initialFrame = 0;
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
        initialFrame = i_frame;
    }

    void set_duration(Uint32 duration){
        this->duration = duration;
        step = duration/totalFrames;
    }

    void set_total_frames(int frames){
        totalFrames = frames;
        step = duration/totalFrames;
    }

    void update(){
        if(stepTimer.getTicks() >= step){
            frame++;
            if(frame == totalFrames){
                finished = true;
                frame = initialFrame;
            }

            stepTimer.stop();
            steps_acomp++;
            stepTimer.start();
            //printf("step\n");
        }

        if(timer.getTicks() >= duration && steps_acomp == totalFrames){
            timer.stop();
            timer.start();

            printf("%d\n",steps_acomp );
            steps_acomp = 0;
            //finished = false;
            printf("restarted\n");
        }

        animationImage.update(frame);
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

    void draw(SDL_Renderer* renderer, SDL_Rect* rct){
        animationImage.draw(renderer, rct);
    }

    bool finished = false;
private:
    Timer timer;
    Timer stepTimer;
    Uint32 duration;
    int frame, totalFrames, step, initialFrame;
    int steps_acomp = 0;
    AnimationImage animationImage;

};


struct Sprite
{


};

struct Player
{
public:
    Player(SDL_Renderer* renderer){
        //bool repite = true;
        //sprite.add("walking", repite)

        //steadyImage = SteadyImage(renderer);
        animations["steady"] = Animation(renderer, "spritesheet.png", 0, 169, 314);
        animations["steady"].set_total_frames(1);
        animations["steady"].set_duration(300);
        animations["shot"] = Animation(renderer, "spritesheet.png", 0, 169, 314);
        animations["shot"].set_total_frames(6);
        animations["shot"].set_duration(1200);
        
    }

    void set_dimensions(int w, int h){
        dst.w = w;
        dst.h = h;
    }

    void set_position(int x, int y){
        dst.x = x;
        dst.y = y;
    }


    void update(){
        if(shooting){
            animations["shot"].update();
        } 
    }

    void draw(SDL_Renderer* renderer){
        if(shooting){
            animations["shot"].draw(renderer, &dst);
        } 
        if(steady){
            animations["steady"].draw(renderer, &dst);  
        }
    }

    void hande_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_a){
                shooting = true;
                steady = false;
            }

        }

    }

public:
    std::unordered_map<std::string, Animation> animations;
    //SteadyImage steadyImage;
    bool shooting = false;
    bool steady = true;

    SDL_Rect dst;

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

        //steadyImage = SteadyImage(renderer);
        animations["steady"] = Animation(renderer, "maleBase/full/advnt_full.png", 0, 32, 64);
        animations["walking"] = Animation(renderer, "maleBase/full/advnt_full.png", 1, 32, 64);
        animations["attacking"] = Animation(renderer, "maleBase/full/attacking.png", 0, 32, 64);
        animations["attacking"].set_total_frames(4);
        animations["attacking"].set_duration(300);
    }

    void set_position(int x, int y){
        dst.x = x;
        dst.y = y;
    }

    void set_dimensions(int w, int h){
        dst.w = w;
        dst.h = h;
    }

    void update(){
        ///VIEW THIS !!!!
        if(!steady){
        //VIEW THIS !!! WALK/ATTTAK ERROR
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

        if(walking){
            dst.x += 2;
        }
    }

    void draw(SDL_Renderer* renderer){
        if(walking){
            animations["walking"].draw(renderer, &dst);
        } 
        if(steady){
            animations["steady"].draw(renderer, &dst);
        }
        if(attacking){
            animations["attacking"].draw(renderer, &dst);
        }
    }

    void hande_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT && !attacking){
                walking = true;
                steady = false;
            }

            if(event.key.keysym.sym == SDLK_a && !attacking){
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
    //SteadyImage steadyImage;
    bool walking = false;
    bool steady = true;
    bool attacking = false;

    SDL_Rect dst;

};

struct State
{
public:
    virtual void handle_sinput(SDL_Event event) = 0;
    virtual void handle_minput(Uint8* state) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
private:
    Animation animation;

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    //Animation animation(game.renderer);
    GameObject animation(game.renderer);

    //animation.set_duration(1000);

    animation.set_dimensions(32, 64);
    animation.set_position(0, 150);

    Player player(game.renderer);

    player.set_dimensions(32*3, 64*3);
    player.set_position(150, 150);

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
            player.hande_input(game.event);
        }

        animation.update();
        animation.draw(game.renderer);

        player.update();
        player.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

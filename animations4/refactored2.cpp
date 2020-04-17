#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>


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

struct Entity;
struct State
{
public:
	virtual ~State() = default;
    virtual State* handle_sinput(Entity* entity, SDL_Event event) = 0;
    virtual State* handle_minput(Entity* entity, const Uint8* key_state) = 0;
    virtual State* update(Entity* entity) = 0;
    virtual void draw(Entity* entity, SDL_Renderer* renderer) = 0;
private:
    Animation animation;

};

struct Entity
{
public:
	virtual ~Entity() = default;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual void handle_sinput(SDL_Event event) = 0;
    virtual void handle_minput(const Uint8* key_state) = 0;

protected:
    State* state = nullptr;
    Animation animation;
    SDL_Rect rct;
    //std::vector<State*> states;
};

struct PlayerState;
struct Player : Entity
{
public:
	//~Player(){}
    
	void set_state(State* _state){
		if(_state != nullptr){
    		delete state;
    		state = _state;
    		printf("state setted\n");
    	}
	}

	void update() override{
		State* new_state = state->update(this);
    	if(new_state != nullptr){
    		delete state;
    		state = new_state;
    	}
	}
    void draw(SDL_Renderer* renderer) override{
    	//animation.draw(renderer, &rct);
    }
    void handle_sinput(SDL_Event event) override{
    	State* new_state = state->handle_sinput(this, event);
    	if(new_state != nullptr){
    		delete state;
    		state = new_state;
    	}
    }
    void handle_minput(const Uint8* key_state) override{
    	State* new_state = state->handle_minput(this, key_state);
    	if(new_state != nullptr){
    		delete state;
    		state = new_state;
    	}
    }	
};

/*struct PlayerState : State
{
public:
    virtual State* handle_sinput(Entity* entity, SDL_Event event) = 0;
    virtual State* handle_minput(Entity* entity, const Uint8* key_state) = 0;
    virtual State* update(Entity* entity) = 0;
    virtual void draw(Entity* entity, SDL_Renderer* renderer) = 0;
};*/

struct SteadyPlayerState : State
{
public:
    State* handle_sinput(Entity* entity, SDL_Event event) override;

    State* handle_minput(Entity* entity, const Uint8* key_state) override;

    State* update(Entity* entity) override;

    void draw(Entity* entity, SDL_Renderer* renderer) override;

};


struct WalkingPlayerState : State
{
public:
	WalkingPlayerState(){}
    State* handle_sinput(Entity* entity, SDL_Event event) override{
        if(event.type == SDL_KEYUP){
            if(event.key.keysym.sym == SDLK_RIGHT){
                printf("UPPPPPPPPPPPPPpp\n");
                return new SteadyPlayerState();
            }
        }

        return nullptr;
    }

    State* handle_minput(Entity* entity, const Uint8* key_state) override{
        printf("updating WalkingPlayerState\n");
    	return nullptr;
    }

    State* update(Entity* entity) override{
    	return nullptr;
    }

    void draw(Entity* entity, SDL_Renderer* renderer) override{

    }
};


State* SteadyPlayerState::handle_sinput(Entity* entity, SDL_Event event){
    return nullptr;
}

State* SteadyPlayerState::handle_minput(Entity* entity, const Uint8* key_state){
    printf("updating SteadyPlayerState\n");
    if (key_state[SDL_SCANCODE_RIGHT]) {
        return new WalkingPlayerState();
    }

    return nullptr;
}

State* SteadyPlayerState::update(Entity* entity){
    return nullptr;
}

void SteadyPlayerState::draw(Entity* entity, SDL_Renderer* renderer){

}



/*
struct SteadyPlayerState : State
{
public:
    State* handle_sinput(Entity* entity, SDL_Event event) override{

 
        return nullptr;
    }

    State* handle_minput(Entity* entity, const Uint8* key_state) override{
    	printf("updating SteadyPlayerState\n");
    	if (key_state[SDL_SCANCODE_RIGHT]) {
    		return new WalkingPlayerState();
        }

        return nullptr;
    }

    State* update(Entity* entity) override{
    	return nullptr;
    }

    void draw(Entity* entity, SDL_Renderer* renderer) override{

    }

};

/*struct AttackingPlayerState : PlayerState
{
	void update() override{

	}
    void draw(SDL_Renderer* renderer) override{

    }
    void handle_sinput(SDL_Event event) override{

    }
    void handle_minput(Uint8* state) override{

    }
};

struct WalkingPlayerState : PlayerState
{
	void update() override{

	}
    void draw(SDL_Renderer* renderer) override{

    }
    void handle_sinput(SDL_Event event) override{

    }
    void handle_minput(Uint8* state) override{

    }
};
*/

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();
    //Animation animation(game.renderer);
    //GameObject animation(game.renderer);

    //animation.set_duration(1000);

    //animation.set_dimensions(32, 64);
    //animation.set_position(0, 150);

    Player player;

   	printf("player created\n");
    player.set_state(new SteadyPlayerState());
    printf("player state setted\n");
    
    //player.set_dimensions(32*3, 64*3);
    //player.set_position(150, 150);

    while(game.running){
        
        SDL_RenderClear(game.renderer);
        
        const Uint8* state = SDL_GetKeyboardState(NULL);
        player.handle_minput(state);
        
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
            player.handle_sinput(game.event);
        
            
        }


        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

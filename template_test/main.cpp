#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

struct GraphicsComponent
{
public:
    virtual void draw() = 0;
};

struct PlayerGraphicsComponent : GraphicsComponent
{
public:
    void draw() override{
        printf("updating PlayerGraphicsComponent\n");
    }
};

struct PhysicsComponent
{
public:
    virtual void update() = 0;
};

struct PlayerPhysicsComponent : PhysicsComponent
{
public:
    void update() override{
        printf("updating PlayerPhysicsComponent\n");
    }
};

template<class TGraphicsComponent,
        //typename InputComponent,
        class TPhysicsComponent>
struct GameObject
{
public:
    GameObject(){

    }

    void update(){
       //iComponent.handle_input();
        pComponent.update();
        gComponent.draw();
    }
private:
    TGraphicsComponent gComponent;
    //InputComponent iComponent;
    TPhysicsComponent pComponent;
};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    SDL_RenderSetScale(game.renderer,1,1);

    SDL_Rect r = {0,0,100,100};

    auto player = GameObject<PlayerGraphicsComponent,
                        PlayerPhysicsComponent>();


    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                
            }
        }
           
        player.update();
        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

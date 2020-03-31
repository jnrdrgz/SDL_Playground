#include "log_system.h"
#include "game.h"
#include <SDL2/SDL.h>
#include "sprite.h"
#include <unordered_map>
#include "bending_bar_controller.h"
#include "gameobject.h"
#include "components/horse/horsegraphicscomponent.h"
#include "components/horse/horseinputcomponent.h"
#include "components/horse/horseupdatecomponent.h"
#include "components/horse/horseaicomponent.h"
#include "components/horse/horsedata.h"

static LogSystem log_system = LogSystem();

class Background
{
public:
    SDL_Texture* image = nullptr;
    int limit = 640*4;
    int laps = 0;

    SDL_Rect src = {0,0,640,480};//tmp_srf->w,tmp_srf->h};
    SDL_Rect dst = {0,0,640,480};
    

    Background(SDL_Renderer* renderer){
        SDL_Surface* tmp_srf = IMG_Load("bt2.png");
        image = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        SDL_FreeSurface(tmp_srf);
    }

    void scroll(int v){
        if(src.x < 640*3){
            if(laps != 10){
                if(src.x < limit-(640*2)){
                    src.x += v;
                } else {
                    src.x = 640;
                    src.x += v;
                    laps++;
                }
            } else {
                src.x += 10;
            }
        }
    }

    //scroll following a reference object or rct
    void reference_scroll(GameObject object){
        if(object.get_rct().x > 640-200){
            scroll(object.get_vel_x());
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, image, &src, &dst);
    }

};


int main(int argc, char* args[])
{
    Game game;

    int screenW = 640;
    int screenH = 480;
    game.init("test", screenW, screenH);
    log_system.init();

    //background
    Background background(game.renderer);

    //game objects
    //Horse horse(game.renderer);

    bool race_started = false;

    HorseData* ECSHorse_data = new HorseData(640*11, 11, 10);
    HorseData* ECSAIHorse_data = new HorseData(640*11, 11, 10);

    GameObject ECSHorse(game.renderer, 
        new HorseGraphicsComponent(ECSHorse_data), 
        new HorseInputComponent(),
        new HorseUpdateComponent(ECSHorse_data));

    ECSHorse.set_size(140,140);
    ECSHorse.set_position(0, 195);
    ECSHorse.set_color_mod(255,0,0);

    GameObject ECSAIHorse(game.renderer, 
        new HorseGraphicsComponent(ECSHorse_data), 
        nullptr,
        new HorseAIComponent(ECSAIHorse_data));

    ECSAIHorse.set_size(140,140);
    ECSAIHorse.set_position(15, 180);

    //controllers
    BendingBarController bender_controller(350, 20, 100, 20);

    log_system.add_text("Laps", std::to_string(background.laps), game.renderer);
    log_system.add_text("ECSHorse", std::to_string(ECSHorse_data->laps_to_finnish), game.renderer);
    log_system.add_text("ECSAIHorse", std::to_string(ECSAIHorse_data->laps_to_finnish), game.renderer);
    //log_system.add_text("Horse_VEL", std::to_string(horse.get_vel_x()), game.renderer);
    //log_system.add_text("Horse_ANIMVEL", std::to_string(horse.get_anim_vel()), game.renderer);

    Uint32 start_time_frame = 0;
    Uint32 dt = 0;

    while(game.running){
        start_time_frame = SDL_GetTicks();
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_SPACE){
                    race_started = true;
                }
            }

            bender_controller.handle_input(game.event);
            ECSHorse.handle_input(game.event);
        }

        if(race_started){
            ECSHorse.update_r(dt);
            ECSAIHorse.update_r(dt);
        }

        background.draw(game.renderer);
        
        //horse.update(game.renderer, dt);
        bender_controller.update();
        background.reference_scroll(ECSHorse);

        bender_controller.draw(game.renderer);

        if(ECSHorse_data->laps_to_finnish == ECSAIHorse_data->laps_to_finnish){
            ECSAIHorse.draw(game.renderer, dt);
        }
        ECSHorse.draw(game.renderer, dt);
        
        log_system.update_text("Laps", std::to_string(background.laps), game.renderer);
        log_system.update_text("ECSHorse", std::to_string(ECSHorse_data->laps_to_finnish), game.renderer);
        log_system.update_text("ECSAIHorse", std::to_string(ECSAIHorse_data->laps_to_finnish), game.renderer);
        //log_system.update_text("Horse_VEL", std::to_string(horse.get_vel_x()), game.renderer);
        //log_system.update_text("Horse_ANIMVEL", std::to_string(horse.get_anim_vel()), game.renderer);

        log_system.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
        dt = (SDL_GetTicks() - start_time_frame);
        
    }
    
    ECSHorse.destroy();
    ECSAIHorse.destroy();
    game.close();
}
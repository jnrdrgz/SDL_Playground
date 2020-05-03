#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"

#include <vector>

static LogSystem log_system = LogSystem();

const int screenw = 640;
const int screenh = 480;

enum class Collision{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

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

struct World;
struct Body
{
public:
    Body(){
        mass = 1.0f;
    }

    Body(World& world);
    Body(int x, int y, int w, int h, bool dynamic, World& world);

    void apply_force(Vector2 force);
    void update(World& world);
    void draw(SDL_Renderer* renderer);

    SDL_Rect rct;
    Vector2 velocity, acceleration, max_vel;
    Vector2 position;
    
    float mass;
    bool active, dynamic;
    int id;
};

struct World
{
public:
    World(){
        limit_xleft = 50; 
        limit_xright = 600;

        limit_yup = 50; 
        limit_ydown = 400;

        limit_rct.x = limit_xleft;
        limit_rct.y = limit_yup;
        limit_rct.w = limit_xright-limit_xleft;
        limit_rct.h = limit_ydown-limit_yup;

    }

    World(Vector2 gravity, Vector2 wind) : World(){
        this->gravity = gravity;
        this->wind = wind;
    }

    void update();
    
    void draw(SDL_Renderer* renderer);


    int limit_xleft, limit_xright;
    int limit_yup, limit_ydown;
    SDL_Rect limit_rct;
    Vector2 gravity, wind;
    std::vector<Body> bodies;
    int current_body_id = 0;
};

void Body::apply_force(Vector2 force){
    Vector2 n_force = force/mass;
    acceleration += force;
    //printf("%.2f\n", acceleration.y);
   
}

void Body::update(World& world){
    
    if(dynamic){
        apply_force(world.gravity);
        apply_force(world.wind);

        if(velocity.x < max_vel.x && velocity.x > -max_vel.x) velocity.x += acceleration.x;
        if(velocity.y > -max_vel.y) velocity.y += acceleration.y;

        //printf("%.2f\n", acceleration.y);

        if((int)position.x < world.limit_xleft){
            position.x = world.limit_xleft;
            velocity.x *= -0.25f;
        }
            
        if((int)position.x+rct.w > world.limit_xright){
            position.x = world.limit_xright-rct.w;
            velocity.x *= -0.25f;
        }
        
        if((int)position.y < world.limit_yup){
            position.y = world.limit_yup;
            velocity.y *= -0.25f;
        }
        if((int)position.y+rct.h > world.limit_ydown){
            position.y = world.limit_ydown-rct.h;
            //velocity.y *= -0.25f;
            velocity.y = 0.0f;
            //printf("%.2f\n", velocity.y);
        }

        for(auto body : world.bodies){
            if(rct_collide(rct, body.rct) && body.id != id && body.active){
                printf("colliding\n");
                velocity.y = -velocity.y;
            }
        }

        position.x += velocity.x;
        position.y += velocity.y;
        
        
        rct.x = position.x;
        rct.y = position.y;

        acceleration.x = 0.0f;
        acceleration.y = 0.0f;
    }
}

void Body::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rct);
}

void World::draw(SDL_Renderer* renderer){
    //draw boundaries
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &limit_rct);

    for(auto& body : bodies){
        body.draw(renderer);
        //printf("body drawed at %d, %d\n", body.rct.x, body.rct.y);
    }
}

void World::update(){
    for(auto& body : bodies){
        body.update(*this);

        //printf("body updated at %d, %d\n", body.rct.x, body.rct.y);

    }
}

Body::Body(World& world): Body(){
    world.bodies.push_back(*this);
    printf("body added\n");
}

Body::Body(int x, int y, int w, int h, bool dynamic, World& world){
    rct.x = x;
    rct.y = y;
    rct.w = w;
    rct.h = h;
    position.x = (float)x;
    position.y = (float)y;
    mass = 1.0f;
    max_vel.x = 4.0f;
    max_vel.y = 4.0f;
    id = world.current_body_id++;
    this->dynamic = dynamic; 
    active = true;

    world.bodies.push_back(*this);  

    printf("body created with id %d\n", world.current_body_id); 
}


int main(int argc, char* args[])
{
    Game game;
    game.init("Plat", screenw, screenh);
    log_system.init();

    Vector2 gravity = Vector2(0.0f,0.03f);
    Vector2 wind = Vector2(0.0f,0.0f);
    World world(gravity, wind);

    Body body_test(200,200,15,15,true,world);
    Body body_test2(260,270,100,15,false,world);

    bool start = false;
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
                    //world.bodies[0].velocity.x = 0.0f;
                    world.bodies[0].apply_force(Vector2(0.5f, 0.0f));
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    //world.bodies[0].velocity.x = 0.0f;
                    world.bodies[0].apply_force(Vector2(-0.5f, 0.0f));
                }
                
                if(game.event.key.keysym.sym == SDLK_s){
                    start = true;
                }

                if(game.event.key.keysym.sym == SDLK_UP){
                    //body_test.apply_force(Vector2(0.0f, 50.0f));
                    world.bodies[0].apply_force(Vector2(0.0f, -2.0f));
                }
            }
        }

        if(start)
           world.update();
        world.draw(game.renderer);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}

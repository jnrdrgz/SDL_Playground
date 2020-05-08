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
    NONE
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

Collision rct_collide_c(SDL_Rect a, SDL_Rect b){
    if( a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y)
    {
        //if(a.y+a.h < b.y+b.h-3) return Collision::UP;
        if(a.y+a.h - 5 < b.y+5) return Collision::UP; 
        if(a.y > b.y) return Collision::DOWN; 
        
        printf("ayw: %d, by: %d\n", a.y+a.w,b.y);
        return Collision::DOWN;
    }

    return Collision::NONE;
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
    bool is_colliding = false;
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

        bodies.reserve(100);
    }

    World(Vector2 gravity, Vector2 wind) : World(){
        this->gravity = gravity;
        this->wind = wind;
    }

    void update();
    
    void draw(SDL_Renderer* renderer);

    Body& create_body(int x, int y, int w, int h, bool dynamic);


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

        if(velocity.x < max_vel.x && velocity.x > -max_vel.x){ 
            velocity.x += acceleration.x;
        }
        if(velocity.y > -max_vel.y){ 
            velocity.y += acceleration.y;
        }
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

            if(body.id != id && body.active){
                Collision col = rct_collide_c(rct, body.rct);
                //printf("colliding\n");
                if(col != Collision::NONE) is_colliding = true;

                if(col == Collision::UP){
                    position.y = body.rct.y-rct.h;
                    velocity.y = 0.0f;
                } else if (col == Collision::DOWN){
                    velocity = -velocity;
                } else {
                    //velocity = -velocity;

                }
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
    max_vel.x = 2.0f;
    max_vel.y = 2.0f;
    id = world.current_body_id++;
    this->dynamic = dynamic; 
    active = true;

    //world.bodies.push_back(*this);  

    printf("body created with id %d\n", world.current_body_id); 

}

Body& World::create_body(int x, int y, int w, int h, bool dynamic){
    //Body body = Body(x, y, w, h, dynamic, *this);
    //printf("to return body %d\n", current_body_id-1);

    //bodies.push_back(Body(x, y, w, h, dynamic, *this));  
    if(current_body_id > 100) throw std::logic_error("maximun body capacity");
    bodies.emplace_back(x, y, w, h, dynamic, *this);
    return bodies.back();
}


int main(int argc, char* args[])
{
    Game game;
    game.init("Plat", screenw, screenh);
    log_system.init();

    Vector2 gravity = Vector2(0.0f,0.05f);
    Vector2 wind = Vector2(0.0f,0.0f);
    World world(gravity, wind);

    //Body body_test(200,200,15,15,true,world);
    Body& body_test = world.create_body(200,200,15,15,true);
    Body& bullet = world.create_body(400,200,5,5,true);
    world.create_body(world.limit_xleft,270,50,15,false);
    world.create_body(world.limit_xright-50,270,50,15,false);

    world.create_body(200,270,50,15,false);
    world.create_body(400,270,50,15,false);
    world.create_body(400,170,50,75,false);

    //world.bodies[0].mass = 5;

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
                    //world.bodies[0].apply_force(Vector2(0.5f, 0.0f));
                    if(body_test.velocity.x < 0) body_test.velocity.x = 0.0f;
                    body_test.apply_force(Vector2(body_test.max_vel.x, 0.0f));
                    printf("acc: %.2f, vel: %.2f\n", body_test.acceleration.x,body_test.velocity.x);
            
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    //world.bodies[0].velocity.x = 0.0f;
                    //world.bodies[0].apply_force(Vector2(-0.5f, 0.0f));
                    if(body_test.velocity.x > 0) body_test.velocity.x = 0.0f;
                    body_test.apply_force(Vector2(-body_test.max_vel.x, 0.0f));
                    printf("acc: %.2f, vel: %.2f\n", body_test.acceleration.x,body_test.velocity.x);
                }
                
                if(game.event.key.keysym.sym == SDLK_s){
                    start = true;
                }

                if(game.event.key.keysym.sym == SDLK_UP){
                    //body_test.apply_force(Vector2(0.0f, 50.0f));
                    //world.bodies[0].apply_force(Vector2(0.0f, -2.0f));
                    body_test.apply_force(Vector2(0.0f, -2.0f));   
                }

                if(game.event.key.keysym.sym == SDLK_x){
                    bullet.velocity.x = 0.0f;
                    bullet.velocity.y = 0.0f;
                    bullet.position.x = body_test.rct.x + body_test.rct.w;
                    bullet.position.y = body_test.rct.y;
                    bullet.apply_force(Vector2(1.0f,-1.5f));    
                }

            }

            if(game.event.type == SDL_KEYUP){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                    //body_test.velocity.x = 0.0f;
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    //body_test.velocity.x = 0.0f;
                }
            }
        }

        if(start) world.update();
        world.draw(game.renderer);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}

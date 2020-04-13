#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

#include <vector>

static LogSystem log_system = LogSystem();

struct Vector2
{
public:
    Vector2():x{0},y{0}
    {

    }

    Vector2(float x, float y):x{x},y{y}
    {

    }

    Vector2 operator+(Vector2 other) const {
        return Vector2(other.x+x, other.y+y);
    }

    Vector2 operator+=(Vector2 other) const {
        return *this+other;
    }

    Vector2 operator-(Vector2 other) const {
        return Vector2(other.x-x, other.y-y);
    } 

    float x,y;

};

struct World;
struct Body
{
public:
    Body(){}
    
    Body(World& world);
    void set_world(World& world);

    void applyForce(Vector2 force){
        acceleration = force;
    }

    void set_position(Vector2 position){
        this->position = position;
    }

    void update(){
        this->position = this->position + acceleration;
    }



    Vector2 position;
    Vector2 acceleration;
    Vector2 speed;
    float mass;

};

struct World
{
    World() : gravity{Vector2(0,9.8f)}
    {

    }

    World(Vector2 gravity) : gravity{gravity}
    {

    }

    void addBody(Body* body){
        bodies.push_back(body);
    }

    void update(){
        for(auto body : bodies){
            body->applyForce(gravity);
            body->update();
            //printf("force: %2.f, %2.f\n", body.position.x, body.position.y);
        }
    }

    void set_limits(float init_x, float init_y, float limit_x, float limit_y){
        this->init_x = init_x;
        this->init_y = init_y;
        this->limit_x = limit_x;
        this->limit_y = limit_y;
    }



    Vector2 gravity;
private:
    float init_x, init_y, limit_x, limit_y; 
    std::vector<Body*> bodies;
};

Body::Body(World& world){
    world.addBody(this);
}

void Body::set_world(World& world){
    world.addBody(this);   
}

struct Ball
{
public:
    Ball(SDL_Renderer* renderer, World& world){
        SDL_Surface* tmp_srf = IMG_Load("ball.png");
        texture = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        SDL_FreeSurface(tmp_srf);


        body = new Body(world);
        //body.set_world(world);

        //temporal
        dst.w = 64;
        dst.h = 64;
    }

    Ball(SDL_Renderer* renderer, World& world, Vector2 position) : Ball(renderer, world){
        //this->position = position;
    }

    Ball(SDL_Renderer* renderer, World& world, float x, float y) : Ball(renderer, world){
        //this->position.x = x;
        //this->position.y = y;
    }

    void set_position(Vector2 position){
        body->set_position(position);
        dst.x = body->position.x;
        dst.y = body->position.y;
    }
    
    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
    }

    void update(){
        //printf("%.2f\n", body->position.y);
        dst.x = body->position.x;
        dst.y = body->position.y;
    }

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect dst; 
    Body* body;
};


int main(int argc, char* args[])
{
    int screen_w = 500, screen_h = 500;

    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    World world{};
    world.set_limits(0.0f, 0.0f, (float)screen_w, (float)screen_h);
    Ball ball{game.renderer, world};

    ball.set_position(Vector2(0.0f,0.0f));

    printf("Gravity %.2f\n", world.gravity.y);
    Vector2 v(2.0f,3.0f);
    Vector2 v1(1.0f,4.0f);
    Vector2 v3 = v + v1;
    
    printf("x:%.2f y:%.2f\n", v3.x, v3.y);
    Vector2 v4(10.0f,10.0f);
    v4 += v3;
    printf("x:%.2f y:%.2f\n", v4.x, v4.y);
    
    Vector2 v5 = v4+v3;
    printf("x:%.2f y:%.2f\n", v5.x, v5.y);
    

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
                    start = true;
                }
            }
        }

        if(start){
            world.update();
            ball.update();
        }
        ball.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

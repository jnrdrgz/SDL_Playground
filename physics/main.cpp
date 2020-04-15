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

    Vector2& operator+=(const Vector2& other) {
        x+=other.x; 
        y+=other.y;

        return *this;
    }

    Vector2 operator-(Vector2 other) const {
        return Vector2(other.x-x, other.y-y);
    } 
    
    Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    Vector2 operator*(float n) const {
        return Vector2(x*n, y*n);
    }

    Vector2 operator/(float n) const {
        return Vector2(x*(1.0f/n), y*(1.0f/n));
    }

    Vector2& operator*=(float n) {
        x*=n; 
        y*=n;

        return *this;
    }

    int mag(){
        return sqrt(x*x + y*y);
    }

    void normalize(){
        float m = mag();
        if (m != 0) {
            x /= m;
            y /= m;
        }
    }

    float x,y;

};

struct World;
struct Body
{
public:
    Body(){

        acceleration = Vector2(0.0f,0.0f);
        velocity = Vector2(0.0f,0.0f);
        mass = 5.0f;
    }
    
    Body(World& world);
    void set_world(World& world);

    void applyForce(Vector2 force){
        acceleration += force;
    }

    void set_position(Vector2 position){
        this->position = position;
    }

    void set_mass(float mass){
        this->mass = mass;
    }

    void update(){
        velocity += acceleration;
        if(position.y>500-64){
            velocity.y*=-1.0f;
            velocity.y+=1.0f; //why
            position.y = 500-64;
            //printf("velocity: %.2f\n", velocity.y);
        }

        if(position.x>500-64){
            velocity.x*=-1.0f;
            velocity.x+=1.0f;
            position.x = 500-64;
            //printf("velocity: %.2f\n", velocity.x);
        }
        //else if(position.y<0){
        //    velocity.y*=-1.0f;
        //    position.y = 0;
        //}
        acceleration *= 0;
        this->position += velocity;
    }

    Vector2 position;
    Vector2 acceleration;
    Vector2 velocity;
    float mass;
    int w,h;

};

struct World
{
    World() : gravity{Vector2(0,0.2f)}
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
            //if(body)
            Vector2 n_g(0.0f,gravity.y/body->mass);

            Vector2 c = body->velocity;
            c *= -1;
            c.normalize();
            c *= friction;

            body->applyForce(c);
            body->applyForce(gravity);
            body->applyForce(wind);

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



private:
    Vector2 gravity;
    Vector2 wind{0.04f, 0.0f};
    float friction = 0.05f;
    float init_x, init_y, limit_x, limit_y; 
    std::vector<Body*> bodies;
};

Body::Body(World& world) : Body(){
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

    void set_mass(float mass){
        body->set_mass(mass);
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


void vector_test(){
    //printf("Gravity %.2f\n", world.gravity.y);
    Vector2 v(2.0f,3.0f);
    Vector2 v1(1.0f,4.0f);
    Vector2 v3 = v + v1;
    
    printf("x:%.2f y:%.2f\n", v3.x, v3.y);
    Vector2 v4(10.0f,10.0f);
    v4 += v3;
    printf("x:%.2f y:%.2f\n", v4.x, v4.y);
    
    Vector2 v5 = -v4;
    printf("x:%.2f y:%.2f\n", v5.x, v5.y);

    Vector2 v6{4.0f, 4.0f};
    v6 = v6/2.0f;
    printf("x:%.2f y:%.2f\n", v6.x, v6.y);
    
    v6 = v6/1.0f;
    printf("x:%.2f y:%.2f\n", v6.x, v6.y);
}

int main(int argc, char* args[])
{
    int screen_w = 500, screen_h = 500;

    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    World world{};
    world.set_limits(0.0f, 0.0f, (float)screen_w, (float)screen_h);
    
    Ball ball{game.renderer, world};
    ball.set_position(Vector2(100.0f,0.0f));
    ball.set_mass(10.0f);
    
    Ball ball1{game.renderer, world};
    ball1.set_position(Vector2(50.0f,200.0f));
    ball1.set_mass(1.0f);

    vector_test();

    bool start = false;

    Uint32 frames = 0;
    Uint32 fps = 0;
    Uint32 i_ftime = SDL_GetTicks();
    Uint32 seconds = 0;
    
    log_system.add_text("SECONDS", std::to_string(seconds), game.renderer);
    
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
            ball1.update();
        }
        ball.draw(game.renderer);
        ball1.draw(game.renderer);


        log_system.update_text("SECONDS", std::to_string(seconds), game.renderer); 
        log_system.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

        if(SDL_GetTicks() - i_ftime > 1000){
            seconds++;
            i_ftime = SDL_GetTicks();
        }
       
    }
    
    
    game.close();
}

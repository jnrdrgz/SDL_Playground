#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"

#include <vector>
#include <algorithm> 

static LogSystem log_system = LogSystem();
int random_between(int mn, int mx){
    int n = rand()%(mx-mn)+mn;
    return n;
}

//template<typename T> 
float random_betweenf(float mn, float mx){
    float n = (float)rand() / (float)RAND_MAX;

    return (mn + (mx - mn) * n);
}

SDL_Color get_random_rainbow_color(){
    //Uint8 numbers[3] = {255,127,0};
    std::vector<Uint8> numbers = {255,127,0};
 
    std::random_shuffle(numbers.begin(), numbers.end());

    Uint8 r = numbers[0];
    Uint8 g = numbers[1];
    Uint8 b = numbers[2];
    /*if(r == 0 && g == 0 && b == 0){
        r = numbers[random()%2];
        g = numbers[random()%2];
        b = numbers[random()%2];
    }*/

    SDL_Color color = {r,g,b,255};

    return color;
}

struct Particle
{
public:
    Particle(){
        life.start();
        active = true;
        gravity = Vector2(0.0f,0.0f);
        speed = Vector2(0.0f,0.0f);
        velocity = Vector2(0.0f,0.0f);
        color = {255,0,0,255};
        lifetime = 1000; //ms
        fade = true;
    }

    Particle(int x, int y, int w, int h, float gy, float wx) : Particle(){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        gravity.y = gy;
        wind.x = wx;
    }
    
    Particle(int x, int y, int w, int h, float gy, float wx, SDL_Color color) : Particle(){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        gravity.y = gy;
        wind.x = wx;

        this->color = color;
    }
    
    
    void update(){
        time_left = lifetime - life.getTicks();
        if(time_left <= 0){
            active = false;
            life.stop();
        }
        
        if(active){
            if(fade) color.a = (255*time_left)/lifetime;
            //color.g = (((255-95)*time_left)/lifetime)+95;

            speed += gravity;
            speed += wind;
            speed += velocity;

            if(rct.x >= 500 || rct.x <= 0){speed.x = 0;}
            if(rct.y >= 400 || rct.y <= 0){speed.y = 0;}

            rct.x += speed.x;
            rct.y += speed.y;
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rct);
    }

    void set_color(Uint8 r,Uint8 g,Uint8 b){
        color.r = r;
        color.g = g;
        color.b = b;
    }

    SDL_Rect rct;
    Vector2 velocity, speed, gravity, wind;
    float mass;
    Uint32 lifetime;
    int time_left;
    Timer life;
    bool active, fade;

    SDL_Color color;
};

struct Emitter
{
public:
    Emitter(){}
    Emitter(Vector2 position){
        this->position.x = position.x;
        this->position.y = position.y;
    }

    void push_particle(int x, int y){
        int d = random_between(5,15);
        Particle p(x,y,d,d,random_betweenf(-0.5f,-0.1f),random_betweenf(-0.1f, 0.04f));
        p.set_color(255,255,0);
        particles.push_back(p);
    }

    void push_particle(){
        int d = random_between(5,15);
        Particle p(position.x,position.y,d,d,random_betweenf(-0.5f,-0.1f),random_betweenf(-0.1f, 0.04f));
        p.set_color(255,255,0);
        particles.push_back(p);
    }

    void push_particle(int d, SDL_Color color){
        Particle p(position.x,position.y,d,d,particles_g,particles_w);
        p.set_color(color.r,color.g,color.b);
        particles.push_back(p);
    }

    void update(){
       	//for(auto& particle : particles){
       	//    particle.update();
       	//}

        particles.erase(std::remove_if(particles.begin(), particles.end(), 
              [&](Particle& p) { p.update(); return !p.active; }),
              particles.end());
    }

    void draw(SDL_Renderer* renderer){
        for(auto particle : particles){
            particle.draw(renderer);
        }
    }
    
    Vector2 position;
    Vector2 velocity, speed, gravity;
    float mass;
    Uint32 lifetime;
    SDL_Color color;
    std::vector<Particle> particles;

    //
    int particles_dimension; //only squares for now
    float particles_g; //gravity
    float particles_w; //wind
    SDL_Color particles_color; //color

};

struct Bullet
{
public:
	Bullet(int x, int y){
		rct.x = x;
		rct.y = y;
		position.x = (float)x;
		position.y = (float)y;

		emitter.position.x = position.x;
		emitter.position.y = position.y;
		rct.w = 7;
		rct.h = 7;
		velocity.x = 5.0f;
		velocity.y = 0.0f;
	}

    void draw(SDL_Renderer* renderer){
    	if(shooted){
    		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        	SDL_RenderFillRect(renderer, &rct);
        	emitter.draw(renderer);
    	}
    }

    void handle_event(SDL_Event event){
    	if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_x){
            	shooted = true;
            }	
        }
    }

    void update(){
    	if(shooted){
    		position += velocity;
    		emitter.position.x = position.x;
    		emitter.position.y = position.y+(rct.h/2);
  
    		emitter.particles_g = 0.05f;//random_betweenf(-0.1f,0.1f);
			emitter.particles_w = random_betweenf(-0.1f, 0.0f);
			emitter.push_particle(random_between(3,8), get_random_rainbow_color());
    		emitter.update();
 
    		rct.x = position.x;
    		rct.y = position.y;
    	}
	}
    
    Emitter emitter;
    Vector2 position;
    Vector2 velocity;
    SDL_Rect rct;
    bool shooted = false;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Bullet test_bullet(0,250);

    std::vector<Bullet> bullets;

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
                if(game.event.key.keysym.sym == SDLK_z){
                	test_bullet.shooted = true;
                }


                if(game.event.key.keysym.sym == SDLK_x){
    				Bullet bullet(0,250);
    				bullet.shooted = true;
    				bullets.push_back(bullet);
                }
            }
        }
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);

        for(auto& bullet : bullets) bullet.update();

        test_bullet.update();
        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        test_bullet.draw(game.renderer);
		
        for(auto bullet : bullets) bullet.draw(game.renderer);
		
		SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}

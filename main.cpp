#include <SDL2/SDL.h>
#include "game.h"
#include <cmath>

#define PI 3.14159265

auto degrees_to_radians = [](double v){return (v * PI)/180;};
auto radians_to_degrees = [](double v){return (v * 180)/PI;};

class Helpers
{
public:
    ///CALC DIAGONAL BULLET VELOCITY
    static double calc_x_vel_angle(int angle){
        bool neg_flag = true;
        
        if(angle > 180){
            angle -= 90;
            neg_flag = false;
        }

        double v = std::sin(degrees_to_radians((double)angle*-1.0));

        if(neg_flag){
            v *= -1;
        }

        return v*10.0;
    }

    static double calc_y_vel_angle(int angle){
        angle *= -1;
        double v = std::cos(degrees_to_radians((double)angle*-1.0));
        return v*10.0;
    }


    static int calculate_angle(SDL_Point p1, SDL_Point p2, SDL_Point p3){
        //int dot = x1*x2 + y1*y2;//      # dot product between [x1, y1] and [x2, y2]
        //int det = x1*y2 - y1*x2;//      # determinant
        //int angle = atan2(det, dot);
        //return angle;

        SDL_Point p4 = {p2.x-p1.x, p2.y-p1.y};
        SDL_Point p5 = {p3.x-p1.x, p3.y-p1.y};

        double angle1 = atan2(p4.x, p4.y) * 180 / PI;
        double angle2 = atan2(p5.x, p5.y) * 180 / PI;

        angle2 = (angle2*-1)-180;
        //printf("%.2f,%.2f\n",angle1,angle2);
        return (int)angle2;
    }

    static int positive(int a){
        return a < 0 ? a*-1 : a;
    }
    
};

class Arrow
{
public:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct = {200,200,50,50};
    int angle = 0;
    
    Arrow(SDL_Renderer *r){
        SDL_Surface* tmp_srf = IMG_Load("arrow.png");
        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        SDL_FreeSurface(tmp_srf);

        
    }
    
    void set_angle(int angle){
        this->angle = angle;
    }

    void draw(SDL_Renderer* r){
        SDL_RenderCopyEx(r, texture, NULL, &rct, angle, 0, SDL_FLIP_NONE);
    }

    void handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEMOTION){
            //printf("%d, %d\n", e.button.x,e.button.y);
        }
    }

    void update(){

    }

};

class Bullet
{
public:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct = {200,200,25,25};
    int angle = 0;
    int x_vel = 0;
    int y_vel = 0;
    bool shooted = false;
    
    Bullet(){
             
    }

    Bullet(SDL_Renderer *r){
        SDL_Surface* tmp_srf = IMG_Load("bullet.png");
        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        SDL_FreeSurface(tmp_srf);        
    }

    void draw(SDL_Renderer* r){
        if(texture){
            SDL_RenderCopyEx(r, texture, NULL, &rct, angle, 0, SDL_FLIP_NONE);
        } else {
            SDL_SetRenderDrawColor( r, 0, 0, 0, 255);
            SDL_RenderFillRect(r, &rct);
        }
    }

    void handle_input(SDL_Event e, int angle){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            printf("%d, %d\n", e.button.x,e.button.y);

            //printf("%d, %d\n", (e.button.x - rct.x)/(rct.y - e.button.y), rct.y - e.button.y);

            //if(rct.x < e.button.x){
            //    x_vel = (e.button.x - rct.x)/(rct.y - e.button.y);
            //    y_vel = 1;
            //}
            //if(rct.x > e.button.x){
            //    x_vel = -5;
            //}
            //if(rct.y > e.button.y){
            //    y_vel = -5;
            //}
            //if(rct.y < e.button.y){
            //    y_vel = 5;
            //}

            if(!shooted){
                x_vel = (int)Helpers::calc_x_vel_angle( (angle*-1)-180);
                y_vel = (int)Helpers::calc_y_vel_angle( (angle*-1)-180);

                shooted = true;
            }
        }
    }

    void update(){
        rct.x += x_vel;
        rct.y += y_vel;

        if(!shooted){

        }

        if(rct.x < 0 || rct.x > 525 || rct.y < 0 || rct.y > 525){
            rct.x = 200;
            rct.y = 200;
            x_vel = 0;
            y_vel = 0;
            shooted = false;
        }
    }

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    Arrow arrow = Arrow(game.renderer);
    Bullet bullet = Bullet();

    int x1 = 500;
    int y1 = 250;

    int angle = 0;

    SDL_Point p1 = {arrow.rct.x, arrow.rct.y+arrow.rct.h};
    SDL_Point p2 = {arrow.rct.x, 0};    
    SDL_Point p3 = {arrow.rct.x, arrow.rct.y+arrow.rct.h};
    SDL_Point p4 = {x1,y1};

    while(game.running){
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                p4.x = game.event.button.x;
                p4.y = game.event.button.y;
                angle = Helpers::calculate_angle(p1,p2,p4);
                //printf("%d\n", angle);
                arrow.set_angle(angle);
                
                //printf("%d\n", (angle*-1)-180);
                //printf("X: %.2f, Y: %.2f\n", calc_x_vel_angle( (angle*-1)-180), calc_y_vel_angle( (angle*-1)-180));
    
                
                //todo
                //change w,h acording line size!!
                //printf("---->%d, %d\n", positive(250-p4.x), positive(250-p4.y));
                
                //////////////////////////////
                
                /*
                if(!bullet.shooted){
                    if( positive(250-p4.y) > positive(250-p4.x)){
                        bullet.rct.h = bullet.rct.w = (250-positive(250-p4.x))/7;
                    } else {
                        bullet.rct.h = bullet.rct.w = (250-positive(250-p4.y))/7;
                    }
                }

                */
                //////////////////////////////
                    //bullet.rct.h = (250-p4.y)/7;
                    //bullet.rct.w = (250-p4.y)/7;
                //} else {
                    //printf("---->%d\n", 250-p4.x);
                    //bullet.rct.h = (250-p4.x)/7;
                    //bullet.rct.w = (250-p4.x)/7;

                //}
                //todo
            }
            arrow.handle_input(game.event);
            bullet.handle_input(game.event, angle);
        }
        
        // switch (game.event.type)
        // {
        //     case SDL_QUIT:
        //         game.running = false;
        //         break;

        //     case SDL_MOUSEMOTION:
        //     break;
        // }

        bullet.update();
        arrow.update();
        arrow.draw(game.renderer);
        bullet.draw(game.renderer);

        SDL_RenderDrawLine(game.renderer, p1.x,p1.y, p2.x, p2.y);
        SDL_RenderDrawLine(game.renderer, p1.x,p1.y, p4.x, p4.y);

        //SDL_RenderDrawPoints(game.renderer, points, 4);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    game.close();
}

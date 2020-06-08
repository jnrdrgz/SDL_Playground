#ifndef ANIMATION_H
#define ANIMATION_H

#include "../../timer.h"
#include "animation_image.h"

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
    };

    //Animation(SDL_Renderer* renderer) : Animation(){
    //    animationImage = AnimationImage{renderer, "maleBase/full/advnt_full.png", 1, 32, 64};
    //}

    Animation(SDL_Renderer* renderer, std::string name, int i_frame, int w, int h) : Animation(){
        animationImage = AnimationImage{renderer, name, i_frame, w, h};
        initialFrame = i_frame;
    }

    // CHANGE PLACE OF ROW FOR FRAMES OR BETTER CHANGE FRAMES FOR A FIXED PLACE IN IMAGE LIKE 5, 8 
    //WOULD BE AN IMAGE FROM THE 5 to 8
    Animation(SDL_Renderer* renderer, std::string name, int i_frame, int row, int frames, int w, int h, int duration) : Animation(){
        animationImage = AnimationImage{renderer, name, i_frame, row, w, h};
        initialFrame = i_frame;
        set_total_frames(frames);
        set_duration(duration);
    }

    Animation(SDL_Renderer* renderer, SDL_Texture* texture, int i_frame, int row, int frames, int w, int h, int duration) : Animation(){
        animationImage = AnimationImage{renderer, texture, i_frame, row, w, h};
        initialFrame = i_frame;
        set_total_frames(frames);
        set_duration(duration);
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
            steps_finished++;
            if(steps_finished > 1) finished = true;
            stepTimer.start();
            
            //printf("-----\n");
            //printf("f:%d, sf:%d\n",frame,steps_finished);
            //printf("step\n");
        }

        if(timer.getTicks() >= duration && steps_finished == totalFrames){
            timer.stop();
            timer.start();

            //printf("%d\n",steps_finished );
            steps_finished = 0;
            //finished = false;
            //printf("restarted\n");
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
         steps_finished = 0;
         finished = false;
    }

    void draw(SDL_Renderer* renderer, SDL_Rect* rct, SDL_RendererFlip flip = SDL_FLIP_NONE){
        animationImage.draw(renderer, rct, flip);       
    }

    bool finished = false;
private:
    Timer timer;
    Timer stepTimer;
    Uint32 duration,step;
    int frame, totalFrames, initialFrame; 
    int steps_finished = 0;
    AnimationImage animationImage;
};

#endif // ANIMATION_H
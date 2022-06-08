#pragma once
#include <SDL.h>
#include "Globals.hpp"

class Lightning {
public:
    Lightning( int strike_pos );
    void present( SDL_Renderer* renderer , SDL_Texture* spotTex , SDL_Texture* strikeTex , 
                    SDL_Rect* spotSrcRec , SDL_Rect* strikeSrcRec );
    bool is_finished() { return finished; }
    SDL_Rect get_strike_area();
private:
    int frame = 0 , totalFrame = 14;
    int frequent = 100 , start;
    int strikePos;
    bool finished = false , is_strike = false;

    int warningFrame = 0 , warningTotalFrame = 4;
    int warningFrequent = 200 , is_warning = false;
};

struct FireRain {
    SDL_Rect firePos;
    int curFrame = 0;
    int timeAppear;
    int fireSpeed;

    FireRain( SDL_Rect newFire , int p_fireSpeed ) {
        firePos = newFire;
        fireSpeed = p_fireSpeed;
        timeAppear = SDL_GetTicks();
    }
};

class Coin {
public:
    Coin( int p_x , int p_y , int p_w , int p_h );
    void animation_present( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void fade_out( SDL_Renderer* renderer , SDL_Texture* bonusTex );
    void start_move();
    void move_up();

    SDL_Rect getRec() { return { x , y , w , h };}
    bool colide = false;
    bool fadeOut_finished = false;
    bool moving = false;

private:
    int x , y , w , h;
    int startTime;
    int curFrame = 0 , base_y , base_x;
    
    int moving_startTime;
    int top_y , top_x; // :<

    int gravity = 200, speed = 100;
    int fadeOut_width = 40, fadeOut_height = 15;
    int fadeOut_startTime;
};

class Button {
public:
    Button( int p_x , int p_y , int p_w , int p_h , SDL_Texture* p_simpleTex , SDL_Texture* p_shinyTex );
    void button_present( SDL_Renderer* renderer );
    bool button_hover();

private:
    int x , y , w , h;
    SDL_Texture* simpleTex , *shinyTex;
};

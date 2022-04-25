#pragma once
#include <SDL.h>

class Lightning {
public:
    Lightning( int strike_pos );
    void present( SDL_Renderer* renderer , SDL_Texture* spotTex , SDL_Texture* strikeTex , SDL_Rect* spotSrcRec , SDL_Rect* strikeSrcRec );
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
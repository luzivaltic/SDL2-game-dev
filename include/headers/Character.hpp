#pragma once

#include<SDL.h>
#include<SDL_image.h>

class Character {
public:
    Character( float x , float y , int width , int height );
    void Jump();
private:
    float x , y;
    int width , height;
};
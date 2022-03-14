#pragma once
#include<SDL.h>
#include<SDL_image.h>

class Entity
{
public:
    Entity(float x , float y , SDL_Texture* tex );
    float getX();
    float getY();
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
    void setX(float val);
    void setY(float val);
private:
    float x , y;
    SDL_Rect currentFrame;
    SDL_Texture* tex;
};


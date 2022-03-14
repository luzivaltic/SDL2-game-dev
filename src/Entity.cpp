#include "Entity.hpp"
#include <SDL.h>
#include <SDL_image.h>

Entity::Entity( float x, float y , SDL_Texture* tex )
:x(x) , y(y) , tex(tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
}

float Entity::getX()
{
    return x;
}
float Entity::getY()
{
    return y;
}
SDL_Texture* Entity::getTex()
{
    return tex;
}
SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}

void Entity::setX( float val )
{
    x = val;
}

void Entity::setY(float val)
{
    y = val;
}

#include<SDL.h>
#include<SDL_image.h>
#include "character.hpp"

Character::Character( float x_p , float y_p , int width_p , int height_p ){
    x = x_p;
    y = y_p;
    width = width_p;
    height = height_p;
}
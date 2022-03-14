#pragma once
#include<SDL.h>
#include<bits/stdc++.h>

using namespace std;


class Vector2d {

public:
    void newJump(int p_speed , int p_a){
        jumpStart = SDL_GetTicks();
        speed = p_speed;
        a = p_a;
    }

    int getHeight(){
        float curTime = (float)( SDL_GetTicks() - jumpStart ) / 1000;
        height = max( curTime * speed - curTime * curTime * a / 2 , (float)0 );
        return height;
    }

private:
    unsigned int jumpStart;
    int speed;
    int a;
    int height;
    bool jumping;
};
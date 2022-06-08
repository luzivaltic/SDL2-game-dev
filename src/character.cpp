#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
#include "character.hpp"
#include "Globals.hpp"

using namespace std;


Character::Character(int p_x , int p_y , int p_w , int p_h ,
    int p_scale, int p_speed , int p_frame_rate , bool p_is_snake,
    int p_iddle_frame , int p_walk_frame ){
    
    x = p_x , y = p_y , w = p_w , h = p_h;
    scale = p_scale;
    speed = p_speed;
    character_frame_rate = p_frame_rate;
    is_snake = p_is_snake;
    iddle_total_frame = p_iddle_frame , walk_max_frame = p_walk_frame;
}

void Character::animation_present( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec) {
    w = srcRec->w * scale , h = srcRec->h * scale; 
    SDL_Rect dstRec = { x , y , w , h };
    
    if( is_snake )
        y = GROUND_HEIGHT - h;
    else 
        dstRec.x = min( SCREEN_WIDTH - w , max( (int)(dstRec.x) , 0 ) ) , x = dstRec.x;
    
    SDL_RenderCopyEx( renderer , animation , srcRec , &dstRec , 0.0 , NULL , flip );
    on_ground = ( (y == GROUND_HEIGHT - h && SDL_GetTicks() - jump_start_time >= 1000 ) || SDL_GetTicks() <= 1000 );
}

void Character::iddle( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec ) {
    animation_present( renderer , animation , &srcRec[iddle_frame] );
    if( SDL_GetTicks() - iddle_start >= 4000 / character_frame_rate )
        iddle_frame = ( iddle_frame + 1 ) % iddle_total_frame , iddle_start = SDL_GetTicks();
}

void Character::walk( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec ){
    int dir = ( flip == SDL_FLIP_NONE ) ? 1 : -1;
    if( SDL_GetTicks() - walk_start >= 1000 / character_frame_rate )
        walk_frame = ( walk_frame + 1 ) % walk_max_frame , walk_start = SDL_GetTicks() , x += dir * speed ;
    if( on_ground )
        animation_present( renderer , animation , &srcRec[walk_frame]);      
}

void Character::startJump( int initital_speed , int p_gravity ) {
    jump_speed = initital_speed;
    gravity = p_gravity;
    jump_start_time = SDL_GetTicks();
    on_ground = false;
}

int Character::getHeight() {
    float curTime = (float)( SDL_GetTicks() - jump_start_time ) / 1000;
    int height = (int)std::max( curTime * jump_speed - curTime * curTime * gravity / 2 , (float)0 );
    return height;
}

void Character::Jump( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec ) {
    jump_next_frame();
    y = GROUND_HEIGHT - srcRec->h * scale - getHeight();
    animation_present( renderer , animation , &srcRec[jump_frame] );
}

void Character::jump_next_frame() {
    if( jump_frame == 0 && SDL_GetTicks() - jump_start_time >= 50 )
        jump_frame++;

    if( jump_frame == 1 && SDL_GetTicks() - jump_start_time >= 100 )
        jump_frame++;
    
    if( jump_frame == 2 && SDL_GetTicks() - jump_start_time >= 200 )
        jump_frame++;
    
    if( jump_frame == 3 && SDL_GetTicks() - jump_start_time >= 600 )
        jump_frame++;
    
    if( jump_frame == 4 && SDL_GetTicks() - jump_start_time >= 700 )
        jump_frame++;
    
    if( jump_frame == 5 && SDL_GetTicks() - jump_start_time >= 950 )
        jump_frame = 0; 
}

void Character::attack( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec ) {
    attack_next_frame();
    repos( srcRec );
    animation_present( renderer , animation , &srcRec[attack_frame] );
}

void Character::repos( SDL_Rect* srcRec ) {
    x = ( flip == SDL_FLIP_NONE ) ? x : tail_pos - srcRec[attack_frame].w * scale;
}

void Character::attack_next_frame(){

    if( attack_frame == 0 && !attacking )
        attacking = true , attack_start = SDL_GetTicks() , tail_pos = ( flip == SDL_FLIP_NONE ) ? x : x + w;

    if( attack_frame == 0 && SDL_GetTicks() - attack_start >= 75 )
        attack_frame++;

    if( attack_frame == 1 && SDL_GetTicks() - attack_start >= 150 )
        attack_frame++;
    
    if( attack_frame == 2 && SDL_GetTicks() - attack_start >= 300 )
        attack_frame++;
    
    if( attack_frame == 3 && SDL_GetTicks() - attack_start >= 1050 )
        attack_frame++;
    
    if( attack_frame == 4 && SDL_GetTicks() - attack_start >= 1275 )
        attack_frame++;
    
    if( attack_frame == 5 && SDL_GetTicks() - attack_start >= 1500 ){
        attack_frame = 0 , attacking = false ;
        x = ( flip == SDL_FLIP_NONE ) ? x : tail_pos - w;
    }
}

void Character::dying( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec ){
    y = GROUND_HEIGHT - srcRec[death_frame].h * scale;
    animation_present( renderer , animation , &srcRec[death_frame] );
    if( SDL_GetTicks() - death_time >= 500 )
        death_frame++, death_time = SDL_GetTicks();
    
    if( death_frame == death_max_frame )
        finished_animation = true , death_frame = 0;
}

void Character::restart_character() {
    x = 0;
    died = false;
    finished_animation = false;
}

#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include "Value.hpp"
#include "Globals.hpp"

class Character {
public:
    Character(int p_x , int p_y , int p_w , int p_h ,
        int p_scale, int p_speed , int p_frame_rate , bool is_snake ,
        int p_iddle_frame , int p_walk_frame );

    void animation_present( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );

    void iddle( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void iddle_next_frame();
    void restart_iddle();

    void walk( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void restart_walk();
    void setFlip( SDL_RendererFlip curFlip ) { flip = curFlip; }

    int getHeight();
    void Jump( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void startJump( int initital_speed , int gravity );
    void jump_next_frame();
    
    bool on_ground = true;

    // attack
    void attack( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void attack_next_frame();
    void repos( SDL_Rect *srcRec );
    bool is_attacking(){ return attacking;};

    // death
    void dying( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void dead() { if( !died ) died = true , death_time = SDL_GetTicks();}
    bool is_dead() { return died;}
    bool is_finished() { return finished_animation;}

    // restart
    void restart_character();

    SDL_Rect getRec() { return SDL_Rect{ x , y , w , h }; }
    SDL_RendererFlip getFlip() {return flip;}
    int get_attack_frame() { return attack_frame; }
    
    int getY() { return y;}
    int getX() { return x; }
    void setX( int pos) { x = pos;}
    void set_ground() { y = GROUND_HEIGHT - h;}

private:
    int x , y , w , h;
    int scale , speed , character_frame_rate;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    bool is_snake;

    // iddle
    int iddle_total_frame;
    int iddle_frame = 0;
    int iddle_start = 0;

    // walk
    int walk_max_frame;
    int walk_start = 0;
    int walk_frame = 0;

    // jump
    int jump_max_frame = 6;
    int jump_frame = 0;
    int jump_start_time = 0;
    int jump_speed = 0 , gravity = 0;
    bool jumping = false;

    // attack
    int attack_max_frame = 6;
    int attack_frame = 0;
    int attack_start = 0;
    bool attacking = false; 
    int tail_pos;

    // death
    int death_frame = 0 , death_max_frame = 6;
    int death_time;
    bool died = false , finished_animation = false;
};
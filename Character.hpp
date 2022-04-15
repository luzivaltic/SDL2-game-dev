#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include "const_value.hpp"

class Character {
public:
    Character(int p_x , int p_y , int p_w , int p_h ,
        int p_scale, int p_speed , int p_frame_rate , bool p_lim ,
        int p_iddle_frame , int p_walk_frame );

    void animation_present( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );

    void iddle( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void iddle_next_frame();
    void restart_iddle();

    void walk( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec , int dir );
    void restart_walk();
    void setFlip( SDL_RendererFlip curFlip ) { flip = curFlip; }

    int getHeight();
    void Jump( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void startJump( int initital_speed , int gravity );
    void jump_next_frame();
    bool on_ground();

    // attack
    void attack( SDL_Renderer* renderer , SDL_Texture* animation , SDL_Rect* srcRec );
    void attack_next_frame();
    void repos( SDL_Rect *srcRec );
    bool is_attacking(){ return attacking;};

    int getX() { return x; }
    SDL_Rect getRec() { return SDL_Rect{ x , y , w , h }; }
    SDL_RendererFlip getFlip() {return flip;}
    int get_attack_frame() { return attack_frame; }

private:
    int x , y , w , h;
    int scale , speed , character_frame_rate;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    bool limit;

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

    // attack
    int attack_max_frame = 6;
    int attack_frame = 0;
    int attack_start = 0;
    bool attacking = false; 
    int tail_pos;
};
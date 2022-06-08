#include<SDL.h>
#include "SpecialEffect.hpp"
#include "Globals.hpp"
#include <bits/stdc++.h>

Lightning::Lightning( int p_strikePos ) {
    strikePos = p_strikePos;
    start = SDL_GetTicks();
    is_warning = true;
}

void Lightning::present( SDL_Renderer* renderer , SDL_Texture* spotTex , SDL_Texture* strikeTex , SDL_Rect* spotSrcRec , SDL_Rect* strikeSrcRec ) {

    if( is_strike ) {
        SDL_Rect dstRec = { strikePos - strikeSrcRec[frame].w / 2 , 0 , strikeSrcRec[frame].w , GROUND_HEIGHT };   

        if( SDL_GetTicks() - start >= frequent ) {
            frame = ( frame + 1 ) % totalFrame , start = SDL_GetTicks();   
            if( frame == 0 )
                finished = true , is_strike = false ;
        }
        SDL_RenderCopy( renderer , strikeTex , &strikeSrcRec[frame] , &dstRec );
    }

    if( is_warning ){
        SDL_Rect dstRec = { strikePos - spotSrcRec[warningFrame].w / 2 , 
                            GROUND_HEIGHT - spotSrcRec[warningFrame].h + 5 , spotSrcRec[warningFrame].w , spotSrcRec[warningFrame].h };
        if( SDL_GetTicks() - start >= warningFrequent ){
            warningFrame = ( warningFrame + 1 ) % warningTotalFrame, start = SDL_GetTicks();   
            if( warningFrame == 0 )
                is_strike = true , is_warning = false;
        }
        SDL_RenderCopy( renderer , spotTex , &spotSrcRec[warningFrame] , &dstRec );
    }
}

SDL_Rect Lightning::get_strike_area(){

    if( 3 < frame && frame < 11)
        return { strikePos - 11 , 0 , 22 , GROUND_HEIGHT };
    return { 0 , 0 , 0 , 0 };
}

Coin::Coin( int p_x , int p_y , int p_w , int p_h ){
    x = p_x, y = p_y;
    w = p_w, h = p_h;  
    startTime = SDL_GetTicks();
}

void Coin::animation_present( SDL_Renderer* renderer ,  SDL_Texture* animation , SDL_Rect* srcRec ){
    SDL_Rect dstRec = { x , y , w , h };
    if( SDL_GetTicks() - startTime >= 250 )
        curFrame = ( curFrame + 1 ) % 4 , startTime = SDL_GetTicks();   
    SDL_RenderCopy( renderer , animation , &srcRec[curFrame] , &dstRec );
}

void Coin::start_move()
{
    if( moving == false )
        moving = true , moving_startTime = SDL_GetTicks(), base_y = y;
}

void Coin::move_up(){
    if( !moving ) return;
    int passTime = SDL_GetTicks() - moving_startTime;

    if( passTime <= 1000 ){
        y = base_y - 100 * passTime / 1000;   
        top_y = y;
    }
    else if( passTime >= 2000 && passTime <= 3000 ){
        y = top_y + (base_y - top_y) * ( passTime - 2000 ) / 1000;  
    }
    else if( passTime >= 3000 ){
        moving = false;
        y = base_y;
    }
}

void Coin::fade_out( SDL_Renderer* renderer , SDL_Texture* bonusTex ) {
    if( !colide )
        colide = true , fadeOut_startTime = SDL_GetTicks() , base_y = y - fadeOut_height;

    int passTime = SDL_GetTicks() - fadeOut_startTime;
    y = base_y - 50 * passTime / 1000;

    if( passTime >= 1000 ){
        fadeOut_finished = true;
    }

    SDL_Rect fadeDst = { x , y , fadeOut_width, fadeOut_height };
    SDL_RenderCopy( renderer , bonusTex , NULL , &fadeDst );
}

Button::Button( int p_x , int p_y , int p_w , int p_h , SDL_Texture* p_simpleTex , SDL_Texture* p_shinyTex ){
    x = p_x , y = p_y , w = p_w , h = p_h;
    simpleTex = p_simpleTex;
    shinyTex = p_shinyTex;   
}

void Button::button_present( SDL_Renderer* renderer )
{
    SDL_Rect dstRec = { x , y, w, h };
    if( button_hover() )
        SDL_RenderCopy( renderer , shinyTex , NULL , &dstRec );
    else{
        SDL_RenderCopy( renderer , simpleTex , NULL , &dstRec );
    }
}

bool Button::button_hover()
{
    int mouse_x , mouse_y;
    SDL_GetMouseState( &mouse_x , &mouse_y );
    return ( ( x <= mouse_x && mouse_x <= x + w ) && ( y <= mouse_y && mouse_y <= y + h ) ); 
}

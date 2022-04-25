#include<SDL.h>
#include "SpecialEffect.hpp"
#include "const_value.hpp"

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
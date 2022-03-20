#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <SDL_mixer.h>
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Vector2d.hpp"

using namespace std;

struct flyObject {
    SDL_Rect Object;
    int dir;
};

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int GAME_LIMIT_WIDTH = 2000;

Mix_Music* takeOver = NULL;
Mix_Chunk* burnIt = NULL;
Mix_Chunk* awake = NULL;
Mix_Chunk* rise = NULL;

int board_state[300];

bool loadMedia()
{
    bool isGood = true;
    
    takeOver = Mix_LoadMUS( "audio/backgroundsound.mp3" );
    if( takeOver == NULL ) {
        cout << "failed here : " << SDL_GetError() << endl;
    }
    Mix_PlayMusic( takeOver , 1 );
    
    return true;
}

int main( int argc, char* args[] )
{
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) > 0 ) 
        cout << "SDL init fail , ERROR: " <<  SDL_GetError() << endl;

    if( !( IMG_Init( IMG_INIT_PNG ) ) )
        cout << "IMG PNG failed , ERROR: " << SDL_GetError() << endl;

    if( !( IMG_Init( IMG_INIT_JPG ) ) )
        cout << "IMG JPG failed , ERROR: " << SDL_GetError() << endl;

    if( Mix_OpenAudio( 44100 , MIX_DEFAULT_FORMAT , 2 , 2048 ) < 0 )
        cout << "SDL_mixer failed , Error: " << Mix_GetError() << endl;

    RenderWindow window( "game v1.0" , SCREEN_WIDTH , SCREEN_HEIGHT );
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    SDL_Texture* backTexture = window.loadTexture("assets/rising_moon.png");
    
    vector<Entity> entities = {
        Entity( 0 , 0 , backTexture ),
        Entity( 30 , 0 , backTexture ),
        Entity( 60 , 0 , backTexture ),
    };
    
    bool gameRunning = true;
    SDL_Event event;

    // limiting frame
    int FPS = 30;
    int frameDelay = 1000 / FPS;

    long long frameStart;
    int frameTime;
    double deg = 0.0;
    
    // const value
    const int TOTAL_FRAME = 4;
    
    const int FRAME_WIDTH = 64;
    const int FRAME_HEIGHT = 205;
    const int DST_FRAME_WIDTH = 32;
    const int DST_FRAME_HEIGHT = 100;

    const int DIRT_WIDTH = 32;
    const int DIRT_HEIGHT = 64;
    const int GROUND_HEIGHT = SCREEN_HEIGHT - DIRT_HEIGHT;

    const int FIRE_WIDTH = DST_FRAME_WIDTH /2;
    const int FIRE_HEIGHT = FIRE_WIDTH;

    const int LEFT_KEY = 0;
    const int RIGHT_KEY = 1;
    const int SPACE_KEY = 2;
    const int FIRE_KEY = 3;
    //

    // character
    SDL_Rect srcRec[TOTAL_FRAME];
    for(int i = 0 ;i < TOTAL_FRAME; i++)
        srcRec[i] = { i * FRAME_WIDTH , 0, FRAME_WIDTH, FRAME_HEIGHT };

    SDL_Rect dstRec = { 0 , GROUND_HEIGHT - DST_FRAME_HEIGHT , DST_FRAME_WIDTH , DST_FRAME_HEIGHT };
    Vector2d curJump;

    int curFrame = 0;
    SDL_RendererFlip curFlip = SDL_FLIP_NONE;

    SDL_Rect fillRect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };

    // firebolt
    vector<flyObject> fireRect;

    // fire raint
    vector<SDL_Rect> fireRain;

    // image prepare
    SDL_Surface* fooSur = IMG_Load("assets/foo_animation.png");
    SDL_Surface* fireSur = IMG_Load("assets/firebolt.jpg");
    SDL_Texture* dirt = window.loadTexture("assets/dirtblock.png");

    SDL_SetColorKey( fooSur , SDL_TRUE , SDL_MapRGB( fooSur->format , 0 , 0xFF , 0xFF ) );
    SDL_SetColorKey( fireSur , SDL_TRUE , SDL_MapRGB( fireSur->format , 255 , 255 , 255 ) );
    SDL_Texture* foo = SDL_CreateTextureFromSurface( window.getRenderer() , fooSur );
    SDL_Texture* fire = SDL_CreateTextureFromSurface( window.getRenderer() , fireSur );

    int stand_still = 0;

    while( gameRunning ) {
        frameStart = SDL_GetTicks();
        int cur_pos = dstRec.x;       
        while( SDL_PollEvent( &event ) ){
            
            if( event.type == SDL_QUIT )
                gameRunning = false;
            else if( event.type == SDL_KEYDOWN ) {
                switch ( event.key.keysym.sym )
                {
                case SDLK_LEFT:
                    board_state[LEFT_KEY] = 1;
                    break;
                case SDLK_RIGHT:
                    board_state[RIGHT_KEY] = 1;
                    break;
                case SDLK_f:
                    board_state[FIRE_KEY] = 1;
                    break;
                case SDLK_SPACE:
                    board_state[SPACE_KEY] = 1;
                    break;
                default:
                    break;
                }
            }

            else if( event.type == SDL_KEYUP ) {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        board_state[LEFT_KEY] = 0;
                        break;
                    case SDLK_RIGHT:
                        board_state[RIGHT_KEY] = 0;
                        break;
                    case SDLK_f:
                        board_state[FIRE_KEY] = 0;
                        break;
                    case SDLK_SPACE:
                        board_state[SPACE_KEY] = 0;
                        break;
                    default:
                        break;
                }
            }
        }
        window.clear();
        
        if( board_state[LEFT_KEY] ) {
            curFrame = ( curFrame + 1 ) % TOTAL_FRAME;
            curFlip = SDL_FLIP_NONE;
            dstRec.x = max( 0 , dstRec.x -  DST_FRAME_WIDTH / 4 );
        }    

        if( board_state[RIGHT_KEY] ) {
            curFrame = ( curFrame + 1 ) % TOTAL_FRAME;
            curFlip = SDL_FLIP_HORIZONTAL;
            dstRec.x = min( GAME_LIMIT_WIDTH , dstRec.x + DST_FRAME_WIDTH / 4);
        }  

        if( board_state[SPACE_KEY] ) {
            if( curJump.getHeight() == 0 )
                curJump.newJump( 300 , 700 );
        }

        SDL_Rect new_dstRec = { min( dstRec.x , ( SCREEN_WIDTH - DST_FRAME_WIDTH ) / 2 ) , dstRec.y , dstRec.w , dstRec.h };

        if( GAME_LIMIT_WIDTH - dstRec.x <= ( SCREEN_WIDTH - DST_FRAME_WIDTH ) / 2 )
            new_dstRec.x = SCREEN_WIDTH - DST_FRAME_WIDTH - ( GAME_LIMIT_WIDTH - dstRec.x ); 

        if( cur_pos == dstRec.x ){
            if( !stand_still )
                stand_still = SDL_GetTicks();
            else if( SDL_GetTicks() - stand_still >= 500 ) {
                curFrame = 1;
                stand_still = 0;
            }
        }

        if( board_state[FIRE_KEY] ) {
            flyObject newObject;
            int curDir;
            if( curFlip == SDL_FLIP_HORIZONTAL )
                curDir = 1;
            else curDir = -1;
            newObject.Object = { new_dstRec.x + DST_FRAME_WIDTH / 2 , new_dstRec.y + DST_FRAME_HEIGHT / 4 , FIRE_WIDTH , FIRE_HEIGHT };
            newObject.dir = curDir;
            fireRect.push_back( newObject );
            board_state[FIRE_KEY] = 0;
        }
        
        // jump
        if( curJump.getHeight() ) curFrame = 1;
        dstRec.y = GROUND_HEIGHT - DST_FRAME_HEIGHT - curJump.getHeight();

        // fill rect
        SDL_SetRenderDrawColor( window.getRenderer() , 32 , 33 , 36 , 255 );
        SDL_RenderFillRect( window.getRenderer() , &fillRect );

        SDL_RenderCopyEx( window.getRenderer() , foo , &srcRec[curFrame] , &new_dstRec , deg , NULL , curFlip );

        // render dirt
        
        int zero_point = max( 0 , dstRec.x - ( SCREEN_WIDTH - DST_FRAME_WIDTH ) / 2 );
        zero_point = min( zero_point , GAME_LIMIT_WIDTH - SCREEN_WIDTH );

        for(int i = 0 ;i < GAME_LIMIT_WIDTH ; i+= DIRT_WIDTH){
            SDL_Rect dirtDst = { i - zero_point , SCREEN_HEIGHT - DIRT_HEIGHT , DIRT_WIDTH , DIRT_HEIGHT };
            SDL_RenderCopy( window.getRenderer() , dirt , NULL , &dirtDst );
        }

        // fire
        for( flyObject &fireBolt : fireRect ){
            fireBolt.Object.x += fireBolt.dir * fireBolt.Object.w;
            SDL_RenderCopy( window.getRenderer() , fire , NULL , &fireBolt.Object );   
        }

        if( !fireRect.empty() && ( fireRect[0].Object.x > SCREEN_WIDTH || fireRect[0].Object.x < 0 ) )
            fireRect.erase( fireRect.begin() );

        // fire rain
        
        SDL_Rect newFire = { rand() % GAME_LIMIT_WIDTH , 0 , 16 , 16 };
        fireRain.push_back( newFire );
        
        for( SDL_Rect &fireBolt : fireRain ){
            fireBolt.y += 8;
            SDL_Rect curFire = fireBolt;
            curFire.x -= zero_point;
            SDL_RenderCopy( window.getRenderer() , fire , NULL , &curFire );    
        }

        if( !fireRain.empty() && ( fireRain[0].y > SCREEN_HEIGHT ) )
            fireRain.erase( fireRain.begin() );

        // limit frame time
        frameTime = SDL_GetTicks() - frameStart;
        if( frameTime < frameDelay )
            SDL_Delay( frameDelay - frameTime );
        //

        window.display();
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}
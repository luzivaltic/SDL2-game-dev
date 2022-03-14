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
    const int DIRT_WIDTH = 32;
    const int DIRT_HEIGHT = 64;
    const int GROUND_HEIGHT = SCREEN_HEIGHT - DIRT_HEIGHT;

    const int LEFT_KEY = 0;
    const int RIGHT_KEY = 1;
    const int SPACE_KEY = 2;
    const int FIRE_KEY = 3;
    //

    // character
    SDL_Rect srcRec[TOTAL_FRAME];
    for(int i = 0 ;i < TOTAL_FRAME; i++)
        srcRec[i] = { i * FRAME_WIDTH , 0, FRAME_WIDTH, FRAME_HEIGHT };

    SDL_Rect dstRec = { 0 , GROUND_HEIGHT - FRAME_HEIGHT , FRAME_WIDTH , FRAME_HEIGHT };
    Vector2d curJump;

    int curFrame = 0;
    SDL_RendererFlip curFlip = SDL_FLIP_NONE;

    SDL_Rect fillRect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };

    // firebolt
    const int FIRE_WIDTH = 32;
    const int FIRE_HEIGHT = 32;
    vector<flyObject> fireRect;

    // image prepare
    SDL_Surface* fooSur = IMG_Load("assets/foo_animation.png");
    SDL_Surface* fireSur = IMG_Load("assets/firebolt.jpg");
    SDL_Texture* dirt = window.loadTexture("assets/dirtblock.png");

    SDL_SetColorKey( fooSur , SDL_TRUE , SDL_MapRGB( fooSur->format , 0 , 0xFF , 0xFF ) );
    SDL_SetColorKey( fireSur , SDL_TRUE , SDL_MapRGB( fireSur->format , 255 , 255 , 255 ) );
    SDL_Texture* foo = SDL_CreateTextureFromSurface( window.getRenderer() , fooSur );
    SDL_Texture* fire = SDL_CreateTextureFromSurface( window.getRenderer() , fireSur );

    while( gameRunning ) {
        frameStart = SDL_GetTicks();
        
        while( SDL_PollEvent( &event ) ){
            flyObject newObject;
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
                    int curDir;
                    if( curFlip == SDL_FLIP_HORIZONTAL )
                        curDir = 1;
                    else curDir = -1;
                    newObject.Object = { dstRec.x + FRAME_WIDTH / 2 , dstRec.y + FRAME_HEIGHT / 4 , FIRE_WIDTH , FIRE_HEIGHT };
                    newObject.dir = curDir;
                    fireRect.push_back( newObject );
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
            dstRec.x = max( 0 , dstRec.x -  FRAME_WIDTH / 4 );
        }    

        if( board_state[RIGHT_KEY] ) {
            curFrame = ( curFrame + 1 ) % TOTAL_FRAME;
            curFlip = SDL_FLIP_HORIZONTAL;
            dstRec.x = min( SCREEN_WIDTH - FRAME_WIDTH , dstRec.x + FRAME_WIDTH / 4);
        }   

        if( board_state[SPACE_KEY] ) {
            if( curJump.getHeight() == 0 )
                curJump.newJump( 300 , 700 );
        }
        
        // jump
        dstRec.y = GROUND_HEIGHT - FRAME_HEIGHT - curJump.getHeight();

        // fill rect
        SDL_SetRenderDrawColor( window.getRenderer() , 32 , 33 , 36 , 255 );
        SDL_RenderFillRect( window.getRenderer() , &fillRect );

        SDL_RenderCopyEx( window.getRenderer() , foo , &srcRec[curFrame] , &dstRec , deg , NULL , curFlip );

        // render dirt
        for(int i = 0 ;i < SCREEN_WIDTH ; i+= DIRT_WIDTH){
            SDL_Rect dirtDst = { i , SCREEN_HEIGHT - DIRT_HEIGHT , DIRT_WIDTH , DIRT_HEIGHT };
            SDL_RenderCopy( window.getRenderer() , dirt , NULL , &dirtDst );
        }

        // fire
        for( flyObject &fireBolt : fireRect ){
            fireBolt.Object.x += fireBolt.dir * fireBolt.Object.w;
            SDL_RenderCopy( window.getRenderer() , fire , NULL , &fireBolt.Object );   
        }

        if( !fireRect.empty() && ( fireRect[0].Object.x > SCREEN_WIDTH || fireRect[0].Object.x < 0 ) )
            fireRect.erase( fireRect.begin() );

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
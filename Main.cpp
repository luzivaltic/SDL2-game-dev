#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <SDL_mixer.h>
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Vector2d.hpp"
#include "Character.hpp"
#include "const_value.hpp"

using namespace std;

Mix_Music* takeOver = NULL;
Mix_Chunk* burnIt = NULL;
Mix_Chunk* awake = NULL;
Mix_Chunk* rise = NULL;

int board_state[300];

struct FireRain {
    SDL_Rect firePos;
    int curFrame = 0;
    int timeAppear;

    FireRain( SDL_Rect newFire ) {
        firePos = newFire;
        timeAppear = SDL_GetTicks();
    }
};

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

bool Snake_colide( Character* Snake , Character* Man )
{
    int head_size = 5;
    SDL_Rect SnakeRec = Snake->getRec();
    SDL_Rect ManRec = Man->getRec();
    SDL_Rect SnakeHead = { 0 , SnakeRec.y , head_size , SnakeRec.h };
    SnakeHead.x = ( Snake->getFlip() == SDL_FLIP_HORIZONTAL ) ? SnakeRec.x + SnakeRec.w - head_size : SnakeRec.x;

    if( max( SnakeHead.x , ManRec.x ) <= min( SnakeHead.x + head_size , ManRec.x + ManRec.w ) )
    if( max( SnakeHead.y , ManRec.y ) <= min( SnakeHead.y + SnakeHead.h , ManRec.y + ManRec.h ) )
        return true;
    
    return false;
}

bool Fire_colide( SDL_Rect Fire , SDL_Rect Man ) {

    if( max( Fire.x , Man.x ) <= min( Fire.x + Fire.w , Man.x + Man.w ) )
    if( max( Fire.y , Man.y ) <= min( Fire.y + Fire.h , Man.y + Man.h ) )
        return true;

    return false;   
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

    bool gameRunning = true;
    SDL_Event event;

    // limiting frame
    int FPS = 60;
    int frameDelay = 1000 / FPS;

    long long frameStart;
    int frameTime;
    double deg = 0.0;
    
    // iddle
    SDL_Texture* iddleTex = window.loadTexture("assets/idle_character.png");
    SDL_Rect iddle_man[10];
    iddle_man[0] = { 0 , 0 , 20 , 38 };
    iddle_man[1] = { 20 , 0 , 21 , 38 };
    iddle_man[2] = { 41 , 0 , 21 , 38 };
    iddle_man[3] = { 62 , 0 , 20 , 38 };

    // walk
    SDL_Texture* walkTex = window.loadTexture("assets/walk_steamMan.png");
    SDL_Rect walk_man[10];
    walk_man[0] = {0 , 0 , 23 , 38 };
    walk_man[1] = {23 , 0 , 16 , 38 };
    walk_man[2] = {39 , 0 , 15 , 38 };
    walk_man[3] = {54 , 0 , 16 , 38 };
    walk_man[4] = {70 , 0 , 13 , 38 };
    walk_man[5] = {83 , 0 , 16 , 38 };

    // jump
    SDL_Texture* jumpTex = window.loadTexture("assets/SteamMan_jump.png");
    SDL_Rect jump_man[10];
    jump_man[0] = {0 , 0 , 26 , 38};
    jump_man[1] = {26 , 0 , 16 , 38};
    jump_man[2] = {42 , 0 , 19 , 38};
    jump_man[3] = {61 , 0 , 19 , 38};
    jump_man[4] = {80 , 0 , 19 , 38};
    jump_man[5] = {99 , 0 , 17 , 38};

    // snake walk
    SDL_Texture* SnakeWalkTex = window.loadTexture("assets/Snake_walk.png");
    SDL_Rect snake_walk[10];
    snake_walk[0] = {0 , 0 , 28 , 17};
    snake_walk[1] = {28 , 0 , 30 , 17};
    snake_walk[2] = {58 , 0 , 31 , 17};
    snake_walk[3] = {89 , 0 , 30 , 17};

    // snake attack
    SDL_Texture* SnakeAttackTex = window.loadTexture("assets/Snake_attack.png");
    SDL_Rect snake_attack[10];
    snake_attack[0] = { 0 , 0 , 28 , 17 };
    snake_attack[1] = { 28 , 0 , 28 , 17 };
    snake_attack[2] = { 56 , 0 , 27 , 17 };
    snake_attack[3] = { 83 , 0 , 42 , 17 };
    snake_attack[4] = { 125 , 0 , 44 , 17 };
    snake_attack[5] = { 169 , 0 , 37 , 17 };

    // fire
    SDL_Surface* FireSur = IMG_Load("assets/fire.png");
    SDL_SetColorKey( FireSur , SDL_TRUE , SDL_MapRGB( FireSur->format , 165 , 165 , 165 ) );
    SDL_Texture* FireTex = SDL_CreateTextureFromSurface( window.getRenderer() , FireSur );
    SDL_Rect fireRec[10];
    vector<FireRain *> fireVec; fireVec.clear();

    for(int i = 0 ;i < 2;  i++)
    for(int j = 0; j < 3; j++)
        fireRec[i*3 + j] = { j * 58 , i * 72 , 51 , 72 };
    
    // background
    SDL_Texture* background = window.loadTexture("assets/Background.png");

    vector<Character *> Snakes;

    Character Man( 0 , GROUND_HEIGHT - iddle_man[0].h * 2 ,
                iddle_man[0].w * 2 , iddle_man[0].h * 2 ,  2 , 10 , 30 , 1 , 4 , 6 );

    // loadMedia();

    // tile
    SDL_Texture* tile = window.loadTexture("assets/Tile.png");
    int snake_appear = SDL_GetTicks();
    int fire_appear = SDL_GetTicks();

    int fireFrame = 0;

    while( gameRunning ) {
        frameStart = SDL_GetTicks();

        int pre_pos = Man.getX();

        while( SDL_PollEvent( &event ) ){
            if( event.type == SDL_QUIT )
                gameRunning = false;
            else if( event.type == SDL_KEYDOWN ){
                switch ( event.key.keysym.sym )
                {
                case SDLK_RIGHT:
                    board_state[RIGHT_KEY] = 1;
                    break;
                case SDLK_LEFT:
                    board_state[LEFT_KEY] = 1;
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
                case SDLK_RIGHT:
                    board_state[RIGHT_KEY] = 0;
                    break;
                case SDLK_LEFT:
                    board_state[LEFT_KEY] = 0;
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

        SDL_RenderCopy(window.getRenderer() , background , NULL , NULL);

        // player movements
        if( board_state[RIGHT_KEY] ) {
            Man.restart_iddle();
            Man.setFlip( SDL_FLIP_NONE );
            Man.walk( window.getRenderer() , walkTex , walk_man , 1 );
        }

        if( board_state[LEFT_KEY] ) {
            Man.restart_iddle();
            Man.setFlip( SDL_FLIP_HORIZONTAL );
            if( !board_state[RIGHT_KEY] )
                Man.walk( window.getRenderer() , walkTex , walk_man , -1 );
        }

        if( board_state[SPACE_KEY] && Man.on_ground() )
                Man.startJump( 400 , 800 );

        if( pre_pos == Man.getX() && Man.on_ground()  
                && !board_state[RIGHT_KEY] && !board_state[LEFT_KEY] ) {
            Man.restart_walk();
            Man.iddle( window.getRenderer() , iddleTex , iddle_man );
        }

        if( !Man.on_ground() ){
            Man.jump_next_frame();
            Man.Jump( window.getRenderer() , jumpTex , jump_man );
        }

        // enermy

        if( SDL_GetTicks() - snake_appear >= 5000 ){
            int num = rand();
            if( num & 1 ){
                Snakes.push_back( new Character( -snake_walk[0].w * 2 , GROUND_HEIGHT - snake_walk[0].h * 2 , 
                        snake_walk[0].w * 2 , snake_walk[0].h * 2 , 2 , 2 , 20 , 0 , 1 , 4 ) );
                Snakes[ Snakes.size() - 1 ]->setFlip( SDL_FLIP_HORIZONTAL );
                // Snake.walk( window.getRenderer() , SnakeWalkTex , snake_walk , 1 );
            }
            else {
                Snakes.push_back( new Character( SCREEN_WIDTH , GROUND_HEIGHT - snake_walk[0].h * 2 , 
                        snake_walk[0].w * 2 , snake_walk[0].h * 2 , 2 , 2 , 20 , 0 , 1 , 4 ) );
                Snakes[ Snakes.size() - 1 ]->setFlip( SDL_FLIP_NONE );
            }
            snake_appear = SDL_GetTicks();
        }

        for( Character * Snake : Snakes ){
            int snakeDir = ( Snake->getFlip() == SDL_FLIP_HORIZONTAL ) ? 1 : -1;
            if( !Snake_colide( Snake , &Man ) && !Snake->is_attacking() )
                Snake->walk( window.getRenderer() , SnakeWalkTex , snake_walk , snakeDir );
            else {
                Snake->attack( window.getRenderer() , SnakeAttackTex , snake_attack );   
            }

            if( Snake_colide( Snake , &Man ) && Snake->get_attack_frame() == 3 )
                printf("you're dead %d\n", fireFrame++ );
        }

        if( !Snakes.empty() ) {
            int snakeDir = ( Snakes[0]->getFlip() == SDL_FLIP_HORIZONTAL ) ? 1 : -1;
            if( snakeDir == 1 && Snakes[0]->getX() > SCREEN_WIDTH ){
                delete *Snakes.begin();
                Snakes.erase( Snakes.begin() );
            }

            if( snakeDir == -1 && Snakes[0]->getX() < -snake_walk[0].w ){
                delete *Snakes.begin();
                Snakes.erase( Snakes.begin() );
            }
        }
        
        // fire
        if( SDL_GetTicks() - fire_appear >= 1000 ) {
            fire_appear = SDL_GetTicks();
            SDL_Rect newFireRec = { rand() % SCREEN_WIDTH , 0 , 17 * 2 , 24 * 2 };
            fireVec.push_back( new FireRain( newFireRec ) );
        }

        for( FireRain * curFire : fireVec ){
            SDL_RenderCopy( window.getRenderer() , FireTex , &fireRec[curFire->curFrame] , &(curFire->firePos) );   
            curFire->firePos.y += 2;
            if( SDL_GetTicks() -  curFire->timeAppear >= 50 )
                curFire->curFrame = ( curFire->curFrame + 1 ) % 6 , curFire->timeAppear = SDL_GetTicks();

            // if( Fire_colide( curFire->firePos , Man.getRec() ) )
                // printf("fire colide\n");
        }

        // tile

        for(int i = 0 ; i <= SCREEN_WIDTH ; i += DIRT_WIDTH){
            SDL_Rect tileDst = { i , GROUND_HEIGHT , DIRT_WIDTH , DIRT_HEIGHT };
            SDL_RenderCopy( window.getRenderer() , tile , NULL , &tileDst );
        }

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
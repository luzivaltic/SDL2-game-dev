#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<bits/stdc++.h>
#include "Graphic.hpp"

using namespace std;


RenderWindow window( "gamev1.0" , SCREEN_WIDTH , SCREEN_HEIGHT );


SDL_Texture* iddleTex, *walkTex , *jumpTex , *deathTex;
SDL_Texture* deathOverlayTex;
SDL_Texture* SnakeWalkTex , *SnakeAttackTex;
SDL_Texture* FireTex;
SDL_Texture* scoreTex;
SDL_Texture* tile;
SDL_Texture* lightningStrikeTex , *lightningSpotTex;
SDL_Texture* CoinTex , *BonusTex;
SDL_Texture* background, *lose_bg, *menu_bg;
SDL_Texture* button_play_simple , *button_guide_simple;
SDL_Texture* button_play_shiny , *button_guide_shiny;
SDL_Texture* guide_bg;
SDL_Texture* god_title;

SDL_Rect iddle_man[10] , walk_man[10] , jump_man[10] , death_man[10];
SDL_Rect snake_walk[10] , snake_attack[10];
SDL_Rect fireRec[10];
SDL_Rect lightningStrikeRec[10] , lightningSpotRec[10];
SDL_Rect CoinRec[10];

TTF_Font* Font = NULL;
Mix_Music* bg_music = NULL;
Mix_Chunk* jump_sound = NULL;

void initialize()
{
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) > 0 ) 
        cout << "SDL init fail , ERROR: " <<  SDL_GetError() << endl;

    if( !( IMG_Init( IMG_INIT_PNG ) ) )
        cout << "IMG PNG failed , ERROR: " << SDL_GetError() << endl;

    if( !( IMG_Init( IMG_INIT_JPG ) ) )
        cout << "IMG JPG failed , ERROR: " << SDL_GetError() << endl;

    if( Mix_OpenAudio( 44100 , MIX_DEFAULT_FORMAT , 2 , 2048 ) < 0 )
        cout << "SDL_mixer failed , Error: " << Mix_GetError() << endl;

    if( TTF_Init() == -1 ){ 
        printf("SDL_ttf fail to initialize! %s\n", TTF_GetError() );
    }
}

void loadGraphic()
{
    bg_music = Mix_LoadMUS( "audio/backgroundsound.mp3" );
    if( bg_music == NULL ) {
        cout << "failed here : " << SDL_GetError() << endl;
    }

    Font = TTF_OpenFont("assets/TitanOne-Regular.ttf", 16);
    if( Font == NULL ) {
        printf("failed to load font\n");
    }

    iddleTex = window.loadTexture("assets/idle_character.png");
    iddle_man[0] = { 0 , 0 , 20 , 38 };
    iddle_man[1] = { 20 , 0 , 21 , 38 };
    iddle_man[2] = { 41 , 0 , 21 , 38 };
    iddle_man[3] = { 62 , 0 , 20 , 38 };

    walkTex = window.loadTexture("assets/walk_steamMan.png");
    walk_man[0] = {0 , 0 , 23 , 38 };
    walk_man[1] = {23 , 0 , 16 , 38 };
    walk_man[2] = {39 , 0 , 15 , 38 };
    walk_man[3] = {54 , 0 , 16 , 38 };
    walk_man[4] = {70 , 0 , 13 , 38 };
    walk_man[5] = {83 , 0 , 16 , 38 };

    jumpTex = window.loadTexture("assets/SteamMan_jump.png");
    jump_man[0] = {0 , 0 , 26 , 38};
    jump_man[1] = {26 , 0 , 16 , 38};
    jump_man[2] = {42 , 0 , 19 , 38};
    jump_man[3] = {61 , 0 , 19 , 38};
    jump_man[4] = {80 , 0 , 19 , 38};
    jump_man[5] = {99 , 0 , 17 , 38};

    deathTex = window.loadTexture( "assets/death_steamMan.png");
    int deathWidth[10] = {20,20, 22,30,39,40} , cur_width = 0;
    for(int i = 0 ; i < 6; i++)
        death_man[i] = { cur_width , 0 , deathWidth[i] , 36 } , cur_width += deathWidth[i];

    deathOverlayTex = window.loadTexture("assets/death_overlay.png");
    SDL_SetTextureBlendMode( deathOverlayTex , SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( deathOverlayTex , 255 * 3 /  4 );

    SnakeWalkTex = window.loadTexture("assets/Snake_walk.png");
    snake_walk[0] = {0 , 0 , 28 , 17};
    snake_walk[1] = {28 , 0 , 30 , 17};
    snake_walk[2] = {58 , 0 , 31 , 17};
    snake_walk[3] = {89 , 0 , 30 , 17};

    SnakeAttackTex = window.loadTexture("assets/Snake_attack.png");
    snake_attack[0] = { 0 , 0 , 28 , 17 };
    snake_attack[1] = { 28 , 0 , 28 , 17 };
    snake_attack[2] = { 56 , 0 , 27 , 17 };
    snake_attack[3] = { 83 , 0 , 42 , 17 };
    snake_attack[4] = { 125 , 0 , 44 , 17 };
    snake_attack[5] = { 169 , 0 , 37 , 17 };

    SDL_Surface* FireSur = IMG_Load("assets/fire.png");
    SDL_SetColorKey( FireSur , SDL_TRUE , SDL_MapRGB( FireSur->format , 165 , 165 , 165 ) );
    FireTex = SDL_CreateTextureFromSurface( window.getRenderer() , FireSur );
    for(int i = 0 ;i < 2;  i++)
    for(int j = 0; j < 3; j++)
        fireRec[i*3 + j] = { j * 58 , i * 72 , 51 , 72 };

    background = window.loadTexture("assets/Background.png");
    lose_bg = window.loadTexture("assets/lose_bg.png");
    scoreTex = window.loadTexture("assets/Score_.png");
    menu_bg = window.loadTexture("assets/Menu_bg.png");
    tile = window.loadTexture("assets/Tile.png");
    guide_bg = window.loadTexture("assets/guide_bg.png");
    god_title = window.loadTexture("assets/title_of_god.png");

    button_play_simple = window.loadTexture("assets/button_play_simple.png");
    button_guide_simple = window.loadTexture("assets/button_guide_simple.png");
    button_play_shiny = window.loadTexture("assets/button_play_shiny.png");
    button_guide_shiny = window.loadTexture("assets/button_guide_shiny.png");


    lightningStrikeTex = window.loadTexture( "assets/lightning_strike.png" );
    lightningSpotTex = window.loadTexture( "assets/lightning_spot.png" );
    for(int i = 0 ; i < 14; i++)
        lightningStrikeRec[i] = { i * 44 , 0 , 44 , 193 };
    for(int i = 0 ;i < 4; i++)
        lightningSpotRec[i] = { i * 44 , 0 , 44 , 36 };

    CoinTex = window.loadTexture("assets/Coin.png");
    for(int i = 0 ; i < 4; i++)
        CoinRec[i] = { (i == 0) ? 0 : CoinRec[i-1].x + 1 + 9 , 0, 9 , 10 };

    // rgb(255, 211, 42)
    SDL_Color BonusColor = { 255 , 211 , 42 };
    SDL_Surface* BonusSur = TTF_RenderText_Solid( Font , "+100" , BonusColor );
    BonusTex = SDL_CreateTextureFromSurface( window.getRenderer() , BonusSur );
    SDL_SetTextureBlendMode( BonusTex , SDL_BLENDMODE_BLEND );

}
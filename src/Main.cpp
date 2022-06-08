#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "RenderWindow.hpp"
#include "Character.hpp"
#include "Value.hpp"
#include "SpecialEffect.hpp"
#include "Globals.hpp"
#include "Graphic.hpp"

using namespace std;

enum displayWindow {
    MENU , GUIDE , PLAY
};

int board_state[300];

vector<FireRain *> fireVec;
vector<Character *> Snakes;
vector<Coin*> CoinVec;
vector<Lightning *> lightningVec;

int snake_appear , fire_appear , lightning_appear , frameStart;
int score = 0 , best_score;
bool pause = false , restart = false;
bool gameRunning = true;
double deg = 0.0;
SDL_Event event;
int current_window = MENU;

Character Man( 0 , GROUND_HEIGHT - 38 * 2 ,
        40 , 38 * 2,  2 , 10 , 30 , 0 , 4 , 6 );

int colide_area( SDL_Rect fir , SDL_Rect sec )
{
    int width = max( 0 , min( fir.x + fir.w , sec.x + sec.w ) - max( fir.x , sec.x ) );   
    int height = max( 0 , min( fir.y + fir.h , sec.y + sec.h ) - max( fir.y , sec.y ) );
    return width * height;
}

bool Snake_colide( Character* Snake , Character* Man )
{
    int head_size = 5;
    int attack_range = 10;

    SDL_Rect SnakeRec = Snake->getRec();
    SDL_Rect SnakeHead = { 0 , SnakeRec.y , head_size + attack_range , SnakeRec.h };
    SnakeHead.x = ( Snake->getFlip() == SDL_FLIP_NONE ) ? SnakeRec.x + SnakeRec.w - head_size : SnakeRec.x;

    if( colide_area( SnakeHead , Man->getRec() ) > 0 ){
        return true;
    }
    return false;
}

bool Fire_colide( SDL_Rect Fire , SDL_Rect Man ) {
    if( colide_area( Fire , Man ) > 20 )
        return true;
    return false;   
}

bool Lightning_colide( SDL_Rect lightning_strikeArea , SDL_Rect Man ) {
    return ( colide_area( Man , lightning_strikeArea ) > Man.h * 5 );
}

bool Coin_colide( SDL_Rect Coin_area , SDL_Rect Man ) {
    return ( colide_area( Coin_area , Man ) > 0 );
}

string convert_number_to_string( int number )
{
    string numberString = "";
    do{
        char num = '0' + number % 10;
        numberString = num + numberString;
        number /= 10;   
    } while( number );  
    return numberString;
}

SDL_Texture* create_texture_from_string( string text , SDL_Color textColor , int* text_with , int* text_height )
{
    SDL_Surface* textSur = TTF_RenderText_Blended( Font , text.c_str() , textColor );   
    SDL_Texture* textTex = SDL_CreateTextureFromSurface( window.getRenderer() , textSur );
    *text_with = textSur->w;
    *text_height = textSur->h;
    return textTex;
}

void add_new_snake()
{
    int num = rand();
    
    if( num & 1 ){
        Snakes.push_back( new Character( -snake_walk[0].w * 2 , GROUND_HEIGHT - snake_walk[0].h * 2 , 
                snake_walk[0].w * 2 , snake_walk[0].h * 2 , 2 , 5 , 20 , 1 , 1 , 4 ) );
    }
    else {
        Snakes.push_back( new Character( SCREEN_WIDTH , GROUND_HEIGHT - snake_walk[0].h * 2 , 
                snake_walk[0].w * 2 , snake_walk[0].h * 2 , 2 , 5 , 20 , 1 , 1 , 4 ) );
        Snakes[ Snakes.size() - 1 ]->setFlip( SDL_FLIP_HORIZONTAL );
    }
    snake_appear = SDL_GetTicks();
}

void add_new_fire()
{
    fire_appear = SDL_GetTicks();
    SDL_Rect newFireRec = { rand() % SCREEN_WIDTH , 0 , 17 * 2 , 24 * 2 };
    fireVec.push_back( new FireRain( newFireRec , 10 ) );
}

void add_new_lightning()
{
    int amount = rand() % 5 + 1;   
    for(int i = 0 ; i < amount; i++){
        int strikePos = rand() % SCREEN_WIDTH;
        lightningVec.push_back( new Lightning( strikePos ) );   
    }
    lightning_appear = SDL_GetTicks();
}

void add_new_coin()
{
    CoinVec.push_back( new Coin( rand() % ( SCREEN_WIDTH / 2 ) , 
                GROUND_HEIGHT - COIN_BASE_HEIGHT , COIN_WIDTH , COIN_HEIGHT ) );

    CoinVec.push_back( new Coin( rand() % ( SCREEN_WIDTH / 2 ) + SCREEN_WIDTH / 2 - COIN_WIDTH , 
                GROUND_HEIGHT - COIN_BASE_HEIGHT , COIN_WIDTH , COIN_HEIGHT ) );
}

string noob = "Noob";
string novice = "Novice";
string master = "Master";
string god = "GOD";

int title_width , title_height ;
SDL_Texture* titleTex;
SDL_Color title_color;

SDL_Color colorWhite = { 255 , 255 , 255 };
SDL_Color lightGray = { 178 , 190 , 195 };
SDL_Color lightBlack = { 45 , 52 , 54 };
SDL_Color black = { 0 , 0 , 0 }; 

void update_current_title()
{
    if( score < 1000 ){
        titleTex = create_texture_from_string( noob , colorWhite , &title_width , &title_height );
    }
    else if( score < 2000 ) {
        titleTex = create_texture_from_string( novice , lightGray , &title_width , &title_height );
    }
    else if( score < 3000 ) {
        titleTex = create_texture_from_string( master , lightBlack , &title_width , &title_height );
    }
    else{ 
        titleTex = create_texture_from_string( god , black , &title_width , &title_height );
    }
}

void score_present( int cur_score ) {

    best_score = max( best_score , cur_score );

    string scoreNum = convert_number_to_string( cur_score );
    string bestScoreNum = convert_number_to_string( best_score );
    string scoreLine = "Score: " + scoreNum;
    string bestScoreLine = "Best Score: " + bestScoreNum;
    string titleCategory = "Title: ";

    int scoreLine_width , scoreLine_height ;
    int bestScoreLine_width , bestScoreLine_height ;
    int titleCategory_width , titleCategory_height ;

    SDL_Texture* scoreLineTex = create_texture_from_string( scoreLine , colorWhite , &scoreLine_width , &scoreLine_height );
    SDL_Texture* bestScoreLineTex = create_texture_from_string( bestScoreLine , colorWhite , &bestScoreLine_width , &bestScoreLine_height );
    SDL_Texture* titleCategoryTex = create_texture_from_string( titleCategory , colorWhite , &titleCategory_width , &titleCategory_height );
    update_current_title();

    SDL_Rect scoreLineRec = { 50 , 10 , scoreLine_width , scoreLine_height };
    SDL_Rect bestScoreLineRec = { 50 , scoreLineRec.y + 5 + scoreLineRec.h , bestScoreLine_width , bestScoreLine_height };
    SDL_Rect titleCategoryRec = { 50 , bestScoreLineRec.y + 5 + scoreLineRec.h , titleCategory_width , titleCategory_height };
    SDL_Rect titleRec = { 50 + titleCategory_width , titleCategoryRec.y , title_width , title_height };

    SDL_RenderCopy( window.getRenderer() , scoreLineTex , NULL , &scoreLineRec );
    SDL_RenderCopy( window.getRenderer() , bestScoreLineTex , NULL , &bestScoreLineRec );
    SDL_RenderCopy( window.getRenderer() , titleCategoryTex , NULL , &titleCategoryRec );
    SDL_RenderCopy( window.getRenderer() , titleTex , NULL , &titleRec );
}

void object_present()
{
    for( Character * Snake : Snakes ){
        if( !Snake_colide( Snake , &Man ) && !Snake->is_attacking() )
            Snake->walk( window.getRenderer() , SnakeWalkTex , snake_walk );
        else {
            Snake->attack( window.getRenderer() , SnakeAttackTex , snake_attack );   
        }

        if( Snake_colide( Snake , &Man ) && Snake->get_attack_frame() == 3 )
            Man.dead();
    }

    for( Coin * curCoin : CoinVec ) {
        if( !Coin_colide( curCoin->getRec() , Man.getRec() ) && !curCoin->colide )
            curCoin->animation_present( window.getRenderer() , CoinTex , CoinRec );
        else 
            curCoin->fade_out( window.getRenderer() , BonusTex );
        
        if( abs( curCoin->getRec().x - Man.getRec().x ) <= 100 )
            curCoin->start_move();

        if( !curCoin->colide )
            curCoin->move_up();
    }

    for( FireRain * curFire : fireVec ){
        SDL_RenderCopy( window.getRenderer() , FireTex , &fireRec[curFire->curFrame] , &(curFire->firePos) );   
        curFire->firePos.y += curFire->fireSpeed;
        if( SDL_GetTicks() -  curFire->timeAppear >= 50 )
            curFire->curFrame = ( curFire->curFrame + 1 ) % 6 , curFire->timeAppear = SDL_GetTicks();

        if( Fire_colide( curFire->firePos , Man.getRec() ) )
            Man.dead();
    }    
    
    for(Lightning * curLightning : lightningVec ) {
        curLightning->present( window.getRenderer() , lightningSpotTex , lightningStrikeTex , lightningSpotRec , lightningStrikeRec );   
        if( Lightning_colide( curLightning->get_strike_area() , Man.getRec() ) )
            Man.dead();
    }        
    
    for(int i = 0 ; i <= SCREEN_WIDTH ; i += DIRT_WIDTH) {
        SDL_Rect tileDst = { i , SCREEN_HEIGHT - DIRT_HEIGHT , DIRT_WIDTH , DIRT_HEIGHT };
        SDL_RenderCopy( window.getRenderer() , tile , NULL , &tileDst );
    }
}

int lightning_frequency = 2000 , fire_frequency = 500 , snake_frequency = 3000;

void apply_level()
{
    if( score / 1000 == 0 ){
        lightning_frequency = 4000;   
        fire_frequency = 1000;
    }
    else if( score / 1000 == 1 ) {
        lightning_frequency = 2000;
        fire_frequency = 500;   
    }
    else {
        lightning_frequency = 1000;
        fire_frequency = 250;
    }
}

void object_handle()
{
    apply_level();

    if( SDL_GetTicks() - snake_appear >= snake_frequency ){
        add_new_snake();
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
    
    if( SDL_GetTicks() - fire_appear >= fire_frequency ) {
        add_new_fire();
    }

    if( !fireVec.empty() )
    if( fireVec[0]->firePos.y > SCREEN_HEIGHT ){
        delete *fireVec.begin();
        fireVec.erase( fireVec.begin() );
    }
    
    if( SDL_GetTicks() - lightning_appear >= lightning_frequency ) {
        add_new_lightning();
    }

    while( !lightningVec.empty() && lightningVec[0]->is_finished() ){
        delete *lightningVec.begin();
        lightningVec.erase( lightningVec.begin() );
    }

    if( CoinVec.empty() ) {
        add_new_coin();
    }

    for( int i = 0 ;i < CoinVec.size(); i++){
        if( CoinVec[i]->fadeOut_finished ){
            score += 100;
            delete CoinVec[i];
            CoinVec.erase( CoinVec.begin() + i );
        }
    }

}

void clear_object()
{
    CoinVec.clear();
    Snakes.clear() , snake_appear = SDL_GetTicks();
    lightningVec.clear() , lightning_appear = SDL_GetTicks();
    Man.restart_character();
    fireVec.clear() , fire_appear = SDL_GetTicks();
}

void player_present()
{
    if( board_state[SPACE_KEY] && Man.on_ground && !Man.is_dead() ){
        Man.startJump( 400 , 800 );
    }

    if( board_state[RIGHT_KEY] && !Man.is_dead() ) {
        Man.setFlip( SDL_FLIP_NONE );
        Man.walk( window.getRenderer() , walkTex , walk_man );
    }

    if( board_state[LEFT_KEY] && !Man.is_dead() ) {
        Man.setFlip( SDL_FLIP_HORIZONTAL );
        if( !board_state[RIGHT_KEY] )
            Man.walk( window.getRenderer() , walkTex , walk_man );
    }

    if( Man.on_ground && !Man.is_dead()  
            && !board_state[RIGHT_KEY] && !board_state[LEFT_KEY] ) {
        Man.iddle( window.getRenderer() , iddleTex , iddle_man );
    }

    if( !Man.on_ground && !Man.is_dead() ){
        Man.Jump( window.getRenderer() , jumpTex , jump_man );
    }

    if( Man.is_dead() ) {
        Man.dying( window.getRenderer() , deathTex , death_man );
        if( Man.is_finished() ){
            pause = true , score = 0;
            SDL_RenderCopy( window.getRenderer() , deathOverlayTex , NULL , NULL );
            SDL_Rect dstLoseBg = { SCREEN_WIDTH / 2 - 200 , SCREEN_HEIGHT / 2 - 200 , 400 , 400 };
            SDL_RenderCopy( window.getRenderer() , lose_bg , NULL , &dstLoseBg );
        }
    }

    if( score == 3000 ) {
        pause = true;
        SDL_RenderCopy( window.getRenderer() , deathOverlayTex , NULL , NULL );
        SDL_Rect dstGodTitle = { SCREEN_WIDTH / 2 - 200 , SCREEN_HEIGHT / 2 - 200 , 400 , 400 };
        SDL_RenderCopy(  window.getRenderer() , god_title , NULL , &dstGodTitle );
    }
}

void player_event_handle()
{
    if( event.type == SDL_KEYDOWN ){
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
            restart = pause;
            break;
        case SDLK_m:
            if( Mix_PausedMusic() )
                Mix_ResumeMusic();
            else
                Mix_PauseMusic();
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
    else if( event.type == SDL_WINDOWEVENT ) {
        switch ( event.window.event )
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window.window_size_change( event.window.data1 , event.window.data2 );
            break;
        default:
            break;
        }   
    }
}

void music_present()
{
    if( !Mix_PlayingMusic() )
        Mix_PlayMusic( bg_music , -1 );
}

void play_screen()
{
    SDL_RenderCopy(window.getRenderer() , background , NULL , NULL);
    object_handle();        
    object_present();
    player_present();
    score_present( score );
    music_present();
}

void delay()
{
    int FPS = 60;
    int frameDelay = 1000 / FPS;
    int frameTime = SDL_GetTicks() - frameStart;
    if( frameTime < frameDelay )
        SDL_Delay( frameDelay - frameTime );
}

void menu_event_handle()
{
    Button playButton( 205, 220 , 120 , 45, button_play_simple , button_play_shiny );
    Button guideButton( 205 , 284 , 120 , 45 , button_guide_simple , button_guide_shiny );

    if( event.type ==  SDL_MOUSEBUTTONDOWN ){
        if( playButton.button_hover() ){
            current_window = PLAY;
            clear_object();
        }
        if( guideButton.button_hover() ) {
            current_window = GUIDE;   
        }
    }   
}

void menu_screen()
{
    Button playButton( 205, 220 , 120 , 45, button_play_simple , button_play_shiny );
    Button guideButton( 205 , 284 , 120 , 45 , button_guide_simple , button_guide_shiny );
    
    SDL_RenderCopy( window.getRenderer() , menu_bg , NULL , NULL );
    playButton.button_present( window.getRenderer() );
    guideButton.button_present( window.getRenderer() );
}

void guide_event_handle()
{
    if( event.key.keysym.sym == SDLK_ESCAPE ){
        current_window = MENU;   
    }
}

void guide_screen()
{
    SDL_RenderCopy( window.getRenderer() , guide_bg , NULL , NULL );
}

void close_all()
{
    Mix_FreeMusic( bg_music );   
}

int main( int argc, char* args[] )
{
    srand( time(nullptr) );
    initialize();
    loadGraphic();
    while( gameRunning ) {
        frameStart = SDL_GetTicks();

        while( SDL_PollEvent( &event ) ){
            if( event.type == SDL_QUIT )
                gameRunning = false;
            else if( current_window == PLAY ){
                window.window_size_change( PLAY_WIDTH , PLAY_HEIGHT );
                player_event_handle();
            }        
            else if( current_window == MENU ){
                window.window_size_change( MENU_SCREEN_WIDTH , MENU_SCREEN_HEIGHT );
                menu_event_handle();
            }
            else {
                window.window_size_change( MENU_SCREEN_WIDTH , MENU_SCREEN_HEIGHT );
                guide_event_handle();
            }
        }

        if( restart ) {
            clear_object();
            pause = false , restart = false;
        }
        if( pause ) continue;

        window.clear();

        if( current_window == PLAY ){
            play_screen();
        }
        else if( current_window == MENU ) {
            menu_screen();   
        }
        else{
            guide_screen();
        }

        delay();
        window.display();
    }
    
    window.cleanUp();
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    close_all();

    return 0;
}
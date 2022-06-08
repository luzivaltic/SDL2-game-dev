#pragma once
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include "RenderWindow.hpp"
#include "Globals.hpp"

extern RenderWindow window;

extern SDL_Texture* iddleTex, *walkTex , *jumpTex , *deathTex;
extern SDL_Texture* deathOverlayTex;
extern SDL_Texture* SnakeWalkTex , *SnakeAttackTex;
extern SDL_Texture* FireTex;
extern SDL_Texture* scoreTex;
extern SDL_Texture* tile;
extern SDL_Texture* lightningStrikeTex , *lightningSpotTex;
extern SDL_Texture* CoinTex , *BonusTex;
extern SDL_Texture* background, *lose_bg , *menu_bg;
extern SDL_Texture* button_play_simple , *button_guide_simple;
extern SDL_Texture* button_play_shiny , *button_guide_shiny;
extern SDL_Texture* guide_bg;
extern SDL_Texture* god_title;

extern SDL_Rect iddle_man[10] , walk_man[10] , jump_man[10] , death_man[10];
extern SDL_Rect snake_walk[10] , snake_attack[10];
extern SDL_Rect fireRec[10];
extern SDL_Rect lightningStrikeRec[10] , lightningSpotRec[10];
extern SDL_Rect CoinRec[10];

extern TTF_Font* Font;
extern Mix_Music* bg_music;
extern Mix_Chunk* jump_sound;

void initialize();
void loadGraphic();
void declare();
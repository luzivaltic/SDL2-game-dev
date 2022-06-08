#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include "RenderWindow.hpp"
#include "Globals.hpp"
#include "Value.hpp"

using namespace std;

RenderWindow::RenderWindow( const char* title , int p_width , int p_height )
    :window(NULL) , renderer(NULL)
{
    window = SDL_CreateWindow( title , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , p_width , p_height , 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

    if( window == NULL )
        cout << "window failed , Error: " << SDL_GetError() << endl;

    renderer = SDL_CreateRenderer( window , -1 , SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC );
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath )
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture( renderer , filePath );

    if( texture == NULL )
        cout << "Failed to load texture. Error : " << SDL_GetError() << endl;

    return texture;
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);   
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::window_size_change(int width , int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    GROUND_HEIGHT = SCREEN_HEIGHT - DIRT_HEIGHT;
    SDL_SetWindowSize( window , width , height );
    SDL_SetWindowPosition( window , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED );
}

SDL_Renderer* RenderWindow::getRenderer() {
    return renderer;
}
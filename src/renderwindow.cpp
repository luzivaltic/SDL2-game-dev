#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;

RenderWindow::RenderWindow( const char* title , int width , int height )
    :window(NULL) , renderer(NULL)
{
    window = SDL_CreateWindow( title , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , width , height , 
        SDL_WINDOW_SHOWN );   

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

void RenderWindow::render( Entity& entity )
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getX() * 4;
    dst.y = entity.getY() * 4;
    dst.w = entity.getCurrentFrame().w * 4;
    dst.h = entity.getCurrentFrame().w * 4;

    SDL_RenderCopy( renderer, entity.getTex() , NULL , &dst );
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer* RenderWindow::getRenderer() {
    return renderer;
}



#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <Entity.hpp>

class RenderWindow
{
public:
    RenderWindow(const char* title , int width , int height);
    SDL_Texture* loadTexture( const char* filePath );
    void cleanUp();
    void clear();
    void render(Entity& entity);
    void display();
    SDL_Renderer* getRenderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
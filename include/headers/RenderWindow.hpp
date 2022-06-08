#pragma once
#include <SDL.h>
#include <SDL_image.h>

class RenderWindow
{
public:
    RenderWindow(const char* title , int p_width , int p_height);
    SDL_Texture* loadTexture( const char* filePath );
    void cleanUp();
    void clear();
    void display();
    void window_size_change(int width , int height);
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow() {return window;}

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
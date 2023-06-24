#pragma once

#include "SDL2/SDL.h"

#include "../../emu_core/inc/graphics.hh"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

class GraphicsDesktop: public Graphics
{
public:
    GraphicsDesktop(){}

    bool init() override
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }

        printf("SDL INIT\n");

        window = SDL_CreateWindow( "Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == nullptr )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1,  0);
        if( renderer == nullptr )
        {
            printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }

        setDrawColour(bgColour);
        // SDL_SetRenderDrawColor(renderer, 0xF0, 0xF0, 0xF0, 0xFF);
        // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_Event event;
        SDL_PollEvent(&event);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        printf("Graphics inited\n");

        return true;
    }

    void draw() override {};

    void setDrawColour(Colour col)
    {
        SDL_SetRenderDrawColor(renderer, colourVals(col).r, colourVals(col).g, colourVals(col).b,colourVals(col).a);
    }

private:
    SDL_Window*   window        { nullptr };
    SDL_Renderer* renderer      { nullptr };
    SDL_Surface*  screenSurface { nullptr };

    Colour        bgColour      { Colour::gameboyGreen };
};
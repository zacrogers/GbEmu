#pragma once


#include "SDL2/SDL.h"
#include "SDL_ttf.h"

#include "../../emu_core/inc/graphics.hh"


typedef struct  {

} DebugState;

typedef struct {
    std::uint8_t buf[32];
} tile_t;


// template<std::uint16_t screenWidth, std::uint16_t screenHeight>
class GraphicsDesktop: public Graphics
{
public:
    static const constexpr std::uint16_t screenWidth      { 500 };
    static const constexpr std::uint16_t screenHeight     { 500 };
    static const constexpr std::uint8_t  tileWidthPx      { 8 };
    static const constexpr std::uint16_t debugPanelWidth  { 280 };

    static const constexpr SDL_Color     debugBgcolor     { 0, 0, 255 };
    static const constexpr SDL_Color     debugFgcolor     { 255, 255, 255 };

    GraphicsDesktop(){}

    // void drawDebugPanel()
    // {
    //     TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 20);

    //     if(!font)
    //     {
    //         printf("y tho TTF_Error: %s\n", TTF_GetError() );
    //         return;
    //     }

    //     const int lineHeight = 25;
    //     int nLines = int(screenHeight/lineHeight);
    //     char* debugInfoString = "0123456789ABC";

    //     SDL_Rect     rects[nLines];
    //     SDL_Texture* textures[nLines];
    //     SDL_Surface* surfaces[nLines];

    //     for(int i = 0; i < nLines; ++i)
    //     {
    //         surfaces[i] = TTF_RenderText_Shaded_Wrapped(font, debugInfoString, debugFgcolor, debugBgcolor, 150);
    //         textures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);

    //         rects[i].x = screenWidth;       // controls the rect's x coordinate
    //         rects[i].y = int(lineHeight*i); // controls the rect's y coordinte
    //         rects[i].w = debugPanelWidth;   // controls the width of the rect
    //         rects[i].h = lineHeight;        // controls the height of the rect

    //         SDL_RenderCopy(renderer, textures[i], NULL, &rects[i]);
    //     }

    //     TTF_CloseFont(font);

    //     for(int i = 0; i < nLines; ++i)
    //     {
    //         SDL_DestroyTexture(textures[i]);
    //         SDL_FreeSurface(surfaces[i]);
    //     }

    //     SDL_RenderPresent(renderer);
    // }


    // void display_tile(SDL_Surface *surface, u16 startLocation, u16 tileNum, int x, int y)
    // {
    //     SDL_Rect rc;

    //     for (int tileY=0; tileY<16; tileY += 2)
    //     {
    //         u8 b1 = bus_read(startLocation + (tileNum * 16) + tileY);
    //         u8 b2 = bus_read(startLocation + (tileNum * 16) + tileY + 1);

    //         for (int bit=7; bit >= 0; bit--)
    //         {
    //             u8 hi = !!(b1 & (1 << bit)) << 1;
    //             u8 lo = !!(b2 & (1 << bit));

    //             u8 color = hi | lo;

    //             rc.x = x + ((7 - bit) * scale);
    //             rc.y = y + (tileY / 2 * scale);
    //             rc.w = scale;
    //             rc.h = scale;

    //             SDL_FillRect(surface, &rc, tile_colors[color]);
    //         }
    //     }
    // }

    // void update_dbg_window()
    // {
    //     int xDraw = 0;
    //     int yDraw = 0;
    //     int tileNum = 0;

    //     SDL_Rect rc;
    //     rc.x = 0;
    //     rc.y = 0;
    //     rc.w = debugScreen->w;
    //     rc.h = debugScreen->h;
    //     SDL_FillRect(debugScreen, &rc, 0xFF111111);

    //     u16 addr = 0x8000;

    //     //384 tiles, 24 x 16
    //     for (int y=0; y<24; y++)
    //     {
    //         for (int x=0; x<16; x++)
    //         {
    //             display_tile(debugScreen, addr, tileNum, xDraw + (x * scale), yDraw + (y * scale));
    //             xDraw += (8 * scale);
    //             tileNum++;
    //         }

    //         yDraw += (8 * scale);
    //         xDraw = 0;
    //     }

    //     SDL_UpdateTexture(sdlDebugTexture, NULL, debugScreen->pixels, debugScreen->pitch);
    //     SDL_RenderClear(sdlDebugRenderer);
    //     SDL_RenderCopy(sdlDebugRenderer, sdlDebugTexture, NULL, NULL);
    //     SDL_RenderPresent(sdlDebugRenderer);
    // }

    bool init() override
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        if(TTF_Init() == -1)
        {
            printf( "TTF could not initialize! TTF_Error: %s\n", TTF_GetError() );
            return false;
        }

        printf("SDL INIT\n");

        auto width = screenWidth + (showDebugInfo ? debugPanelWidth : 0);

        window = SDL_CreateWindow( "Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, screenHeight, SDL_WINDOW_SHOWN );
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
        SDL_RenderClear(renderer);

        // SDL_SetRenderDrawColor(renderer, 0xF0, 0xF0, 0xF0, 0xFF);
        // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // drawDebugPanel();

        SDL_Event event;
        bool quit = false;

        while (!quit)
        {
            SDL_WaitEvent(&event);

            switch (event.type)
            {
                case SDL_TEXTINPUT:
                    quit = true;
                    break;
            }
        }



        // SDL_Event event;
        // SDL_PollEvent(&event);
        // // SDL_RenderClear(renderer);


        // SDL_Delay(1000);


        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();

        printf("Graphics inited\n");

        return true;
    }

    void draw() override {};

    void setDrawColour(Colour col)
    {
        SDL_SetRenderDrawColor(renderer, colourVals(col).r, colourVals(col).g, colourVals(col).b, colourVals(col).a);
    }

private:
    SDL_Window*   window        { nullptr };
    SDL_Renderer* renderer      { nullptr };
    SDL_Surface*  screenSurface { nullptr };

    Colour        bgColour      { Colour::gameboyGreen0 };

    bool          showDebugInfo { false };

    colour_vals_t frameBuffer[screenWidth * screenHeight * tileWidthPx];
};
#pragma once

#include "etl/map.h"

// namespace Emu::Graphics
// {}

typedef union {
    struct { std::uint8_t r, g, b, a };
    std::uint32_t rgba;
} colour_vals_t;

// union { struct { std::uint8_t a, f; }; std::uint16_t af; };
enum class Colour
{
    red, green, blue, gameboyGreen0, gameboyGreen1, gameboyGreen2, gameboyGreen3
};

static const etl::map<Colour, colour_vals_t, 100> colours =
{
    {Colour::gameboyGreen0, {139, 172, 15, 255}},
    {Colour::gameboyGreen1, {139, 172, 15, 255}},
    {Colour::gameboyGreen2, {139, 172, 15, 255}},
    {Colour::gameboyGreen3, {139, 172, 15, 255}},
    {Colour::red,           {0xFF, 0, 0, 255}},
    {Colour::green,         {0, 0xFF, 0, 255}},
    {Colour::blue,          {0, 0, 0xFF, 255}}
};

colour_vals_t colourVals(Colour c)
{
    auto it = colours.find(c);
    return it->second;
}

colour_vals_t gameboyGreenLevel[] = {
    colourVals(Colour::gameboyGreen0),
    colourVals(Colour::gameboyGreen1),
    colourVals(Colour::gameboyGreen2),
    colourVals(Colour::gameboyGreen3),
};




// template<std::uint16_t screenWidth, std::uint16_t screenHeight>
class Graphics
{
public:
    virtual bool init() = 0;
    virtual void draw() = 0;
    virtual void drawPixel() = 0;
};

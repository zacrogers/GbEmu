#pragma once

#include "etl/map.h"

// namespace Emu::Graphics
// {}

typedef struct {
    std::uint8_t r, g, b, a;
} colour_vals_t;


enum class Colour
{
    red, green, blue, gameboyGreen
};

static const etl::map<Colour, colour_vals_t, 100> colours =
{
    {Colour::gameboyGreen, {139, 172, 15, 255}},
    {Colour::red,          {0xFF, 0, 0, 255}},
    {Colour::green,        {0, 0xFF, 0, 255}},
    {Colour::blue,         {0, 0, 0xFF, 255}}
};


colour_vals_t colourVals(Colour c)
{
    auto it = colours.find(c);
    return it->second;
}

class Graphics
{
public:
    virtual bool init() = 0;
    virtual void draw() = 0;
};

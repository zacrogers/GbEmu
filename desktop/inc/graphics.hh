#pragma once

#include "../../emu_core/inc/graphics.hh"


class GraphicsDesktop: public Graphics
{
public:
    GraphicsDesktop(){}

    bool init() override { return true ;};
    void draw() override {};

};
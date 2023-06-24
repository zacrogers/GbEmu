#include <iostream>


// #define LOGGING_ENABLED

#include "../inc/etl_profile.h"

#include "../../emu_core/inc/cpu.hh"
#include "../../emu_core/inc/registers.hh"
#include "../../emu_core/inc/bus.hh"
#include "../../emu_core/inc/cart.hh"
#include "../../emu_core/inc/graphics.hh"

#include "../inc/graphics.hh"

class Emulator
{
public:
    Emulator(Graphics *g): pGraphics(g){}

    bool init()
    {
        bool success = true;
        cpu.init();
        // success = pGraphics->init();
        return success;
    }

    int run(int argc, char **argv)
    {
        if (argc < 2)
        {
            printf("Usage: emu <rom_file>\n");
            return -1;
        }

        if (!cart.load(argv[1]))
        {
            printf("Failed to load ROM file: %s\n", argv[1]);
            return -2;
        }

        printf("Cart loaded..\n");


        // TTF_Init();
        // printf("TTF INIT\n");

        // cpu_init();

        running = true;
        paused  = false;
        ticks   = 0;

        auto times = 0;

        while(running)
        {
            if (paused)
            {
                // delay(10);
                continue;
            }

            if (!cpu.step())
            {
                printf("CPU Stopped\n");
                return -3;
            }
            SDL_Delay(100);

            if(times++ > 100) return 0;

            ticks++;
        }

        return 0;
    }

private:
    Graphics          *pGraphics;

    DMG01::Cart       cart    { };
    DMG01::Registers  reg     { };
    DMG01::Bus        bus     { &cart };
    DMG01::Cpu        cpu     { &reg, &bus };

    bool              running { false };
    bool              paused  { false };
    uint64_t          ticks   { 0 };

};

GraphicsDesktop dtGraphics { };
Emulator        emu        { &dtGraphics };


int main(int argc, char **argv)
{
    if(emu.init())
    {
        return emu.run(argc, argv);
    }
    return 0;
}
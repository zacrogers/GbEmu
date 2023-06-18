#include <iostream>

#define LOGGING_ENABLED

#include "dmg01.hh"
#include "cart.hh"


class Emulator
{
public:
    Emulator(){}

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

        // SDL_Init(SDL_INIT_VIDEO);
        // printf("SDL INIT\n");
        // TTF_Init();
        // printf("TTF INIT\n");

        // cpu_init();

        running = true;
        paused  = false;
        ticks   = 0;

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

            ticks++;
        }

        return 0;
    }

private:
    DMG01::Cpu  cpu     { };
    DMG01::Cart cart    { };
    bool        running { false };
    bool        paused  { false };
    uint64_t    ticks   { 0 };

};

Emulator emu { };


int main(int argc, char **argv)
{
    return emu.run(argc, argv);
}
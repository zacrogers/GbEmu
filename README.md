## Mini handheld
- ESP32


### Zephyr project

` west update `

- states
- subsystems
- connectivity
- games
- utilities

### Electronics
- V1


## Gameboy Emulator - WIP

Following tutorial at  https://www.youtube.com/watch?v=e87qKixKFME

Important References:

https://gbdev.io/pandocs/

https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

https://archive.org/details/GameBoyProgManVer1.1/page/n85/mode/2up

https://github.com/rockytriton/LLD_gbemu/raw/main/docs/The%20Cycle-Accurate%20Game%20Boy%20Docs.pdf

https://github.com/rockytriton/LLD_gbemu/raw/main/docs/gbctr.pdf



Current progress PART 02:
    NEED TO FIX!!!

    zac@zac-iMac:~/Documents/GbEmu/my_emu/build$ gbemu/gbemu  ../../roms/dmg-acid2.gb
    Opened: ../../roms/dmg-acid2.gb
    Cartridge Loaded:
            Title    : DMG-ACID2
            Type     : 00 (ROM ONLY)
            ROM Size : 32 KB
            RAM Size : 00
            LIC Code : 00 (None)
            ROM Vers : 00
            Checksum : 9F (PASSED)
    Cart loaded..
    SDL INIT
    TTF INIT
    Exectuting instruction 00    PC: 0101
    Not executing yet...
    Unknown addressing mode: 14
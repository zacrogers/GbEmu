#include <bus.h>

/*
    0x0000 - 0x3FFF : ROM Bank 0
    0x4000 - 0x7FFF : ROM Bank 1 - switchable
    0x8000 - 0x97FF : CMR RAM
    0x9800 - 0x9BFF : BG Map 1
    0x9C00 - 0x9FFF : BG Map 2
    0xA000 - 0xBFFF : Cartridge RAM
    0xC000 - 0xCFFF : RAM Bank 0
    0xD000 - 0xDFFF : RAM Bank 1-7 switchable - Colour only
    0xE000 - 0xFDFF : Reserved - Echo RAM
    0xFE00 - 0xFE9F : Object attribute memory
    0xFEA0 - 0xFEFF : Reserved - Unusable
    0xFF00 - 0xFF7F : I/O registers
    0xFF80 - 0xFFFE : Zero Page
*/

u8 bus_read(u16 address)
{
    if(address < 0x8000)
    {
        // Rom Data
        return cart_read(address);
    }

    NO_IMPL
}

void bus_write(u16 address, u8 value)
{
    if(address < 0x8000)
    {
        cart_write(address, value);
        return;
    }
    NO_IMPL
}
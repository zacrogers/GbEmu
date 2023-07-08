#pragma once

#include "../inc/common.hh"
#include "../inc/cart.hh"


namespace DMG01
{
namespace MemMap {
    constexpr address_t busSize          = 0xFFFF;
    constexpr address_t wramSize         = 0x2000;
    constexpr address_t vramSize         = 0x2000;
    constexpr address_t hramSize         = 0x0080;

    constexpr address_t bootRom          = 0x0000;
    constexpr address_t gameRomBank0     = 0x0000;
    constexpr address_t gameRomBankN     = 0x4000;
    constexpr address_t tileRam          = 0x8000;
    constexpr address_t backgroundMap    = 0x9800;
    constexpr address_t cartridgeRam     = 0xA000;
    constexpr address_t workingRam       = 0xC000;
    constexpr address_t echoRam          = 0xE000;
    constexpr address_t OAM              = 0xFE00; // object attribute memory
    constexpr address_t IO               = 0xFF00;
    constexpr address_t highRam          = 0xFF80;
    constexpr address_t interruptEnabled = 0xFFFF;
};

class Bus
{
public:
Bus(Cart* cart) : pCart(cart){}

word_t read(address_t address)
{
    if      (address < MemMap::tileRam)      { return pCart->read(address); }         /* ROM Data */
    else if (address < MemMap::cartridgeRam) { return vramRead(address); }            /* Char/Map Data */
    else if (address < MemMap::workingRam)   { return pCart->read(address); }         /* Cartridge RAM */
    else if (address < MemMap::echoRam)      { return wramRead(address);  }           /* WRAM (Working RAM) */
    else if (address < MemMap::OAM)          { return 0; }                            /* reserved echo ram... */
    else if (address < 0xFEA0)               { }                                      /* OAM */
    else if (address < 0xFF00)               { return 0; }                            /* reserved unusable... */
    else if (address < 0xFF80)               { }                                      /* IO Registers... */
    else if (address == 0xFFFF)              { /* return cpu_get_ie_register(); */ }  /* CPU ENABLE REGISTER... */

    return hramRead(address);
}
const std::uint16_t read16(address_t address)
{
    return read(address) | (read(address + 1) & 0xFF) << 8;
}

void write(const address_t addr, const word_t val)
{
    bus[addr] = val;
}

std::uint8_t wramRead(address_t address) {
    address -= 0xC000;

    if (address >= 0x2000) {
        printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
        exit(-1);
    }

    return wram[address];
}

void wramWrite(address_t address, std::uint8_t value) {
    address -= 0xC000;

    wram[address] = value;
}

std::uint8_t hramRead(address_t address) {
    address -= 0xFF80;

    return hram[address];
}

void hramWrite(address_t address, std::uint8_t value)
{
    hram[address - 0xFF80] = value;
}

void vramWrite(address_t address, std::uint8_t value)
{
    vram[address - 0x8000] = value;
}

std::uint8_t vramRead(address_t address)
{
    return vram[address - 0x8000];
}



private:
    word_t bus[MemMap::busSize] { };
    Cart *pCart;

    std::uint8_t wram[MemMap::wramSize];
    std::uint8_t vram[MemMap::vramSize];
    std::uint8_t hram[MemMap::hramSize];
};
} // namespace DMG01

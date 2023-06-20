#pragma once

#include "../inc/common.hh"
#include "../inc/cart.hh"

namespace DMG01
{
namespace MemMap {
    static constexpr address_t busSize          = 0xFFFF;
    static constexpr address_t bootRom          = 0x0000;
    static constexpr address_t gameRomBank0     = 0x0000;
    static constexpr address_t gameRomBankN     = 0x4000;
    static constexpr address_t tileRam          = 0x8000;
    static constexpr address_t backgroundMap    = 0x9800;
    static constexpr address_t cartridgeRam     = 0xA000;
    static constexpr address_t workingRam       = 0xC000;
    static constexpr address_t echoRam          = 0xE000;
    static constexpr address_t OAM              = 0xFE00; // object attribute memory
    static constexpr address_t IO               = 0xFF00;
    static constexpr address_t highRam          = 0xFF80;
    static constexpr address_t interruptEnabled = 0xFFFF;
};

class Bus
{
public:
Bus(Cart* cart) : pCart(cart){}
word_t read(address_t address)
{
    if      (address < MemMap::tileRam)      { return pCart->read(address); }         /* ROM Data */
    else if (address < MemMap::cartridgeRam) {  }                                     /* Char/Map Data */
    else if (address < MemMap::workingRam)   { return pCart->read(address); }         /* Cartridge RAM */
    else if (address < MemMap::echoRam)      { /* return wram_read(address); */ }     /* WRAM (Working RAM) */
    else if (address < MemMap::OAM)          { return 0; }                            /* reserved echo ram... */
    else if (address < 0xFEA0)               { }                                      /* OAM */
    else if (address < 0xFF00)               { return 0; }                            /* reserved unusable... */
    else if (address < 0xFF80)               { }                                      /* IO Registers... */
    else if (address == 0xFFFF)              { /* return cpu_get_ie_register(); */ }  /* CPU ENABLE REGISTER... */

    return bus[address];
}
const std::uint16_t read16(address_t addr)
{
    return (std::uint16_t)&bus[addr];
}

void write(const address_t addr, const word_t val)
{
    bus[addr] = val;
}
private:
    word_t bus[MemMap::busSize] { };
    Cart *pCart;
};
} // namespace DMG01

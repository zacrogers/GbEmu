#pragma once

#include"common.hh"

namespace DMG01
{
namespace MemMap {
    static constexpr std::uint16_t busSize          = 0xFFFF;
    static constexpr std::uint16_t bootRom          = 0x0000;
    static constexpr std::uint16_t gameRomBank0     = 0x0000;
    static constexpr std::uint16_t gameRomBankN     = 0x4000;
    static constexpr std::uint16_t tileRam          = 0x8000;
    static constexpr std::uint16_t backgroundMap    = 0x9800;
    static constexpr std::uint16_t cartridgeRam     = 0xA000;
    static constexpr std::uint16_t workingRam       = 0xC000;
    static constexpr std::uint16_t echoRam          = 0xE000;
    static constexpr std::uint16_t OAM              = 0xFE00; // object attribute memory
    static constexpr std::uint16_t IO               = 0xFF00;
    static constexpr std::uint16_t highRam          = 0xFF80;
    static constexpr std::uint16_t interruptEnabled = 0xFFFF;
};

class Bus
{
public:
word_t* read(address_t addr)
{
    return &bus[addr];
}
std::uint16_t* read16(address_t addr)
{
    return (std::uint16_t*)&bus[addr];
}

void write(const address_t addr, const word_t val)
{
    bus[addr] = val;
}
private:
    word_t bus[MemMap::busSize] { };
};
} // namespace DMG01

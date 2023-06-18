#pragma once


#include"common.hh"


namespace DMG01
{
class Registers
{
public:
    void setFlags(const Flags *flags)
    {
        word_t val = 0;

        val |= ((std::uint8_t)flags->carry     << 4);
        val |= ((std::uint8_t)flags->halfCarry << 5);
        val |= ((std::uint8_t)flags->subtract  << 6);
        val |= ((std::uint8_t)flags->zero      << 7);

        set8(Register::F, val & 0xFF00); // Lower nibble must always be zeros
    }


    address_t pcIncr()              { return pc++; }
    void      pcSet(address_t addr) { pc = addr; }
    address_t pcGet()               { return pc; }

    void get();

    void set(const Register reg, const std::uint16_t val)
    {
        switch (reg)
        {
        case Register::A:
        case Register::F:
        case Register::B:
        case Register::C:
        case Register::D:
        case Register::E:
        case Register::H:
        case Register::L:
            set8(reg, (std::uint8_t)(val & 0xFF00));
            break;

        case Register::AF:
        case Register::BC:
        case Register::DE:
            set16(reg, val);
            break;

        case Register::Count:
        case Register::SP:
        case Register::PC:
        case Register::None:
        default:
            break;
        }

    }
// private:
    static constexpr std::uint8_t regOffset = static_cast<std::uint8_t>(Register::AF);

    std::uint8_t  buf[static_cast<int>(Register::Count)] { }; // registers
    address_t     pc  { 0 };
    address_t     sp  { 0 };

    void set8(const Register reg, const std::uint8_t val){ buf[static_cast<std::uint8_t>(reg)] = val; }
    void set16(const Register reg, const std::uint16_t val)
    {
        std::uint16_t* p { reinterpret_cast<std::uint16_t*>(buf) };
        p[static_cast<std::uint8_t>(reg) - regOffset] = val;

    }

    inline word_t get8(const Register reg) { return buf[static_cast<uint8_t>(reg)]; };
    inline word_t get16(const Register reg)
    {
        std::uint16_t* p { reinterpret_cast<std::uint16_t*>(buf) };
        return p[static_cast<std::uint8_t>(reg) - regOffset];
    }
};
}

#include "../inc/registers.hh"
#include <stdio.h>


namespace DMG01
{
/* Public */
void Registers::setFlags(const Flags *flags)
{
    word_t val = 0;

    val |= ((std::uint8_t)flags->carry     << 4);
    val |= ((std::uint8_t)flags->halfCarry << 5);
    val |= ((std::uint8_t)flags->subtract  << 6);
    val |= ((std::uint8_t)flags->zero      << 7);

    set8(Register::F, val & 0xFF00); // Lower nibble must always be zeros
}



void Registers::set(const Register reg, const std::uint16_t val)
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


void Registers::print()
{
        printf("A: 0x%02x F: 0x%02x B: 0x%02x C: 0x%02x D: 0x%02x E: 0x%02x H: 0x%02x L: 0x%02x SP:%04x ",
            get8(Register::A),
            get8(Register::F),
            get8(Register::B),
            get8(Register::C),
            get8(Register::D),
            get8(Register::E),
            get8(Register::H),
            get8(Register::L),
            sp
        );
}

address_t Registers::pcIncr()              { return pc++; }
void      Registers::pcSet(address_t addr) { pc = addr; }
address_t Registers::pcGet()               { return pc; }


/* Private */
void Registers::set8(const Register reg, const std::uint8_t val){ buf[static_cast<std::uint8_t>(reg)] = val; }
void Registers::set16(const Register reg, const std::uint16_t val)
{
    std::uint16_t* p { reinterpret_cast<std::uint16_t*>(buf) };
    p[static_cast<std::uint8_t>(reg) - regOffset] = val;

}

word_t Registers::get8(const Register reg) { return buf[static_cast<uint8_t>(reg)]; };
std::uint16_t Registers::get16(const Register reg)
{
    std::uint16_t* p { reinterpret_cast<std::uint16_t*>(buf) };
    return p[static_cast<std::uint8_t>(reg) - regOffset];
}
}
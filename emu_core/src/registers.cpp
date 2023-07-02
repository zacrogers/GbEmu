
#include "../inc/registers.hh"
#include <stdio.h>


namespace DMG01
{
// offset to apply when accessing the 16 bit registers
// static constexpr std::uint8_t regOffset = static_cast<std::uint8_t>(Register::AF);

/* Public */
void Registers::setFlags(const Flags *flags)
{
    // printf("SET FLAGS");
    word_t val = 0;

    val |= (((std::uint8_t)flags->carry ? 1 : 0 )    << 4);
    val |= (((std::uint8_t)flags->subtract ? 1 : 0)  << 5);
    val |= (((std::uint8_t)flags->halfCarry ? 1 : 0) << 6);
    val |= (((std::uint8_t)flags->zero      ? 1 : 0) << 7);

    write(Register::F, val); // Lower nibble must always be zeros
}



bool Registers::is16(const Register reg)
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
        return false;
    case Register::AF:
    case Register::BC:
    case Register::DE:
    case Register::HL:
        return true;

    case Register::Count:
    case Register::SP:
    case Register::PC:
    case Register::None:
    default:
        return false;
    }

}
void Registers::write(const Register reg, const std::uint16_t val)
{
    switch (reg)
    {
    case Register::A: regs.a = (std::uint8_t)val; break;
    case Register::F: regs.f = (std::uint8_t)val; break;
    case Register::B: regs.b = (std::uint8_t)val; break;
    case Register::C: regs.c = (std::uint8_t)val; break;
    case Register::D: regs.d = (std::uint8_t)val; break;
    case Register::E: regs.e = (std::uint8_t)val; break;
    case Register::H: regs.h = (std::uint8_t)val; break;
    case Register::L: regs.l = (std::uint8_t)val; break;

    case Register::AF:regs.af = val; break;
    case Register::BC:regs.bc = val; break;
    case Register::DE:regs.de = val; break;
    case Register::HL:regs.hl = val; break;

    case Register::Count:
    case Register::SP:
    case Register::PC:
    case Register::None:
    default:
        break;
    }

}
std::uint16_t Registers::read(const Register reg)
{
    switch (reg)
    {
    case Register::A:  return regs.a;
    case Register::F:  return regs.f;
    case Register::B:  return regs.b;
    case Register::C:  return regs.c;
    case Register::D:  return regs.d;
    case Register::E:  return regs.e;
    case Register::H:  return regs.h;
    case Register::L:  return regs.l;
    case Register::AF: return regs.af;
    case Register::BC: return regs.bc;
    case Register::DE: return regs.de;
    case Register::HL: return regs.hl;
    case Register::Count:
    case Register::SP:
    case Register::PC:
    case Register::None:
    default:
        break;
    }
    return 0;
}


void Registers::print()
{
        printf("AF: 0x%04x BC: 0x%04x DE: 0x%04x HL: 0x%04x SP:%04x ",
            read(Register::AF),
            read(Register::BC),
            read(Register::DE),
            read(Register::HL),
            sp
        );
        // printf("A: 0x%02x F: 0x%02x B: 0x%02x C: 0x%02x D: 0x%02x E: 0x%02x H: 0x%02x L: 0x%02x SP:%04x ",
        //     read(Register::A),
        //     read(Register::F),
        //     read(Register::B),
        //     read(Register::C),
        //     read(Register::D),
        //     read(Register::E),
        //     read(Register::H),
        //     read(Register::L),
        //     sp
        // );
}

address_t Registers::pcIncr()              { return pc++; }
void      Registers::pcSet(address_t addr) { pc = addr; }
address_t Registers::pcGet()               { return pc; }
void      Registers::spSet(address_t addr) { sp = addr; }
address_t Registers::spGet()               { return sp; }


/* Private */
// void Registers::set8(const Register reg, const std::uint8_t val){ buf[static_cast<std::uint8_t>(reg)] = val; }
// void Registers::set16(const Register reg, const std::uint16_t val)
// {
//     printf("SET16 VAL: 0x%04x  :: %d\n", val, static_cast<std::uint8_t>(reg) - regOffset);
//     buf[static_cast<std::uint8_t>(reg) - regOffset]     = (val >> 8);
//     buf[static_cast<std::uint8_t>(reg) - regOffset + 1] = val & 0xFF;
// }

// word_t Registers::get8(const Register reg) { return buf[static_cast<uint8_t>(reg)]; };
// std::uint16_t Registers::get16(const Register reg)
// {
//     std::uint16_t* p { reinterpret_cast<std::uint16_t*>(buf) };
//     return p[static_cast<std::uint8_t>(reg) - regOffset];
// }
}
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "common.hh"
#include "instructions.hh"

namespace DMG01
{



static constexpr std::uint8_t regOffset = static_cast<std::uint8_t>(Register::AF);

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

struct Flag {
    bool zero, subtract, halfCarry, carry;
};


class Core
{
public:
    void          setRegister(const Register reg, const std::uint16_t val)
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
            set8BitRegister(reg, (std::uint8_t)(val & 0xFF00));
            break;

        case Register::AF:
        case Register::BC:
        case Register::DE:
            set16BitRegister(reg, val);
            break;

        case Register::Count:
        default:
            break;
        }
    }

    address_t execute(opcode_t *opcode)
    {
        auto inst = Instruction::fetch(*opcode);

        if(inst->mnemonic == Instruction::MN::ADD)
        {
            add(inst);
        }
        if(inst->mnemonic == Instruction::MN::NOP)
        {

        }

        return 0;
    }

    void add(const Instruction::ctx *inst)
    {
        auto opA = static_cast<std::uint16_t>(getRegisterValue(inst->regA));
        auto opB = static_cast<std::uint16_t>(getRegisterValue(inst->regB));

        std::uint16_t result = opA + opB;

        // TODO: check all the flags and actually set them in the reg
        f.zero  = (result == 0 )   ? true : false;
        f.carry = (result > 0xFF ) ? true : false;

        set8BitRegister(Register::A, static_cast<std::uint8_t>(result & 0xFF00));
    }

    // void add(Register reg)
    // {
    //     // cast to u16 to account for possible overflow
    //     auto opA = static_cast<std::uint16_t>(getRegisterValue(Register::A));
    //     auto opB = static_cast<std::uint16_t>(getRegisterValue(reg));

    //     std::uint16_t result = opA + opB;

    //     // TODO: check all the flags and actually set them in the reg
    //     f.zero  = (result == 0 )   ? true : false;
    //     f.carry = (result > 0xFF ) ? true : false;

    //     set8BitRegister(Register::A, static_cast<std::uint8_t>(result & 0xFF00));
    // }

    // void addhl(Register reg)
    // {
    //     // cast to u16 to account for possible overflow
    //     auto opA = static_cast<std::uint16_t>(getRegisterValue(Register::A));
    //     auto opB = static_cast<std::uint16_t>(getRegisterValue(reg));

    //     std::uint16_t result = opA + opB;

    //     // TODO: check all the flags
    //     f.zero  = (result == 0 )   ? true : false;
    //     f.carry = (result > 0xFF ) ? true : false;

    //     set16BitRegister(Register::HL, result);
    // }


    void step()
    {
        static word_t*   pWord  { &bus[pc] };
        static address_t nextPc { 0 };

        if((pWord != nullptr) && Instruction::exists((opcode_t)*pWord))
        {
            nextPc = execute(static_cast<opcode_t*>(pWord));
        }
    }

private:
    std::uint8_t  r[static_cast<int>(Register::Count)] { }; // registers
    word_t        bus[MemMap::busSize]                 { };
    address_t     pc                                   { 0 };
    address_t     sp                                   { 0 };
    Flag          f                                    { }; // flags

    // these should only be called by setRegister. No range checking is done
    void set8BitRegister(const Register reg, const std::uint8_t val){ r[static_cast<std::uint8_t>(reg)] = val; }
    void set16BitRegister(const Register reg, const std::uint16_t val)
    {
        std::uint16_t* p { reinterpret_cast<std::uint16_t*>(r) };
        p[static_cast<std::uint8_t>(reg) - regOffset] = val;

    }

    inline word_t getRegisterValue(const Register reg) { return r[static_cast<uint8_t>(reg)]; };
};

}
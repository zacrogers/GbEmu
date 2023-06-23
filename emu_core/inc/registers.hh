#pragma once

#include"common.hh"

namespace DMG01
{
class Registers
{
public:
    Registers()
    {
        set16(Register::AF, 0xB001);
        set16(Register::BC, 0x1300);
        set16(Register::DE, 0xD800);
        set16(Register::HL, 0x4D01);
    }


    void          setFlags  (const Flags *flags);
    void          set       (const Register reg, const std::uint16_t val);
    void          print     ();
    address_t     pcIncr    ();
    void          pcSet     (address_t addr);
    address_t     pcGet     ();
    void          spSet     (address_t addr);
    address_t     spGet     ();

    void          set8      (const Register reg, const std::uint8_t val);
    void          set16     (const Register reg, const std::uint16_t val);

    word_t        get8      (const Register reg);
    std::uint16_t get16     (const Register reg);

    void setOpA(std::uint16_t val) { opA = val; }
    void setOpB(std::uint16_t val) { opB = val; }
    std::uint16_t getOpA() { return opA; }
    std::uint16_t getOpB() { return opB; }

    // to keep track if the dest of the current instruction is a memory address
    // set when fetching data, and checked executing when executing an instruction
    address_t         memDest     { 0 };
    bool              destIsMem   { false };

private:
    std::uint8_t  buf[static_cast<int>(Register::Count)] { }; // registers
    address_t     pc  { 0x100 };  // Program counter
    address_t     sp  { 0xFFFE }; // Stack pointer

    // Place to store the fetched operand results corresponding to regA & regB in Instruction::ctx
    std::uint16_t opA { 0 };
    std::uint16_t opB { 0 };

};
}
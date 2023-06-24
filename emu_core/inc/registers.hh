#pragma once

#include"common.hh"

namespace DMG01
{
class Registers
{
public:


    Registers()
    {
        write(Register::AF, 0xB001);
        write(Register::BC, 0x1300);
        write(Register::DE, 0xD800);
        write(Register::HL, 0x4D01);
    }


    void          setFlags  (const Flags *flags);
    void          write     (const Register reg, const std::uint16_t val);
    std::uint16_t read      (const Register reg);
    void          print     ();
    address_t     pcIncr    ();
    void          pcSet     (address_t addr);
    address_t     pcGet     ();
    void          spSet     (address_t addr);
    address_t     spGet     ();
    address_t     spDecrement     () { return sp--;}
    address_t     spIncrement     () { return sp++;}

    void setOpA(std::uint16_t val) { opA = val; }
    void setOpB(std::uint16_t val) { opB = val; }
    std::uint16_t getOpA() { return opA; }
    std::uint16_t getOpB() { return opB; }

    // to keep track if the dest of the current instruction is a memory address
    // set when fetching data, and checked executing when executing an instruction
    address_t         memDest     { 0 };
    bool              destIsMem   { false };

private:
    struct registers
    {
		union { struct { std::uint8_t f, a; }; std::uint16_t af; };
		union { struct { std::uint8_t c, b; }; std::uint16_t bc; };
		union { struct { std::uint8_t e, d; }; std::uint16_t de; };
		union { struct { std::uint8_t l, h; }; std::uint16_t hl; };
    };

    struct registers regs {};


    // std::uint8_t  buf[static_cast<int>(Register::Count)] { }; // registers
    address_t     pc  { 0x100 };  // Program counter
    address_t     sp  { 0xFFFE }; // Stack pointer

    // Place to store the fetched operand results corresponding to regA & regB in Instruction::ctx
    std::uint16_t opA { 0 };
    std::uint16_t opB { 0 };

};
}
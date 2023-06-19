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

    void          set8      (const Register reg, const std::uint8_t val);
    void          set16     (const Register reg, const std::uint16_t val);

    word_t        get8      (const Register reg);
    std::uint16_t get16     (const Register reg);


private:
    static constexpr std::uint8_t regOffset = static_cast<std::uint8_t>(Register::AF);

    std::uint8_t  buf[static_cast<int>(Register::Count)] { }; // registers
    address_t     pc  { 0x100 };
    address_t     sp  { 0xFFFE };

};
}
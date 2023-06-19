#pragma once

#include"common.hh"

namespace DMG01
{
class Registers
{
public:
    void      setFlags (const Flags *flags);
    void      set      (const Register reg, const std::uint16_t val);
    void      print    ();
    address_t pcIncr   ();
    void      pcSet    (address_t addr);
    address_t pcGet    ();

    void set8(const Register reg, const std::uint8_t val);
    void set16(const Register reg, const std::uint16_t val);

    word_t get8(const Register reg);
    std::uint16_t get16(const Register reg);


private:
    static constexpr std::uint8_t regOffset = static_cast<std::uint8_t>(Register::AF);

    std::uint8_t  buf[static_cast<int>(Register::Count)] { }; // registers
    address_t     pc  { 0 };
    address_t     sp  { 0 };

};
}
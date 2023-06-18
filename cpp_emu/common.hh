#pragma once

#include <cstdint>
namespace DMG01
{

typedef std::uint16_t address_t;
typedef std::uint8_t  word_t;
typedef std::uint8_t  opcode_t;


enum class Register {
    // AF, BC, DE and HL occupy the same memory so don't count them.
    // They are just added here for access convenience
    A, F, B, C, D, E, H, L, Count, AF, BC, DE, HL, None
};


}
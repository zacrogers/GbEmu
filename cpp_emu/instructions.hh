#pragma once

#include <cstdint>
#include "common.hh"

namespace DMG01
{
namespace Instruction {

enum class Mnemonic {
    NONE,
    NOP,
    LD,
    INC,
    DEC,
    RLCA,
    ADD,
    RRCA,
    STOP,
    RLA,
    JR,
    RRA,
    DAA,
    CPL,
    SCF,
    CCF,
    HALT,
    ADC,
    SUB,
    SBC,
    AND,
    XOR,
    OR,
    CP,
    POP,
    JP,
    PUSH,
    RET,
    CB,
    CALL,
    RETI,
    LDH,
    JPHL,
    DI,
    EI,
    RST,
    ERR,
    //CB instructions...
    RLC,
    RRC,
    RL,
    RR,
    SLA,
    SRA,
    SWAP,
    SRL,
    BIT,
    RES,
    SET
};

enum class AddrMode {
    IMP,
    R_D16, // src: d16, dest: r16
    R_R,   // src: r8,  dest: r8
    MR_R,
    R,
    R_D8,  // src: d8,  dest: r8
    R_MR,
    R_HLI,
    R_HLD,
    HLI_R,
    HLD_R,
    R_A8,
    A8_R,
    HL_SPR,
    D16,
    D8,
    D16_R,
    MR_D8,
    MR,
    A16_R,
    R_A16
};



typedef struct {
    Mnemonic     mnemonic;
    AddrMode     addrMode;
    Register     regA;
    Register     regB;
    std::uint8_t mCycles; // should actually be t states
    std::size_t  len;
    std::uint8_t param;
} ctx ;


typedef Register R;
typedef AddrMode AM;
typedef Mnemonic MN;

static const ctx inst[0x100] = {
//   OP    :    TYPE    | ADDR MODE |  R1   |   R2    | MC | LEN
//  0x00
    [0x00] =  { MN::NOP, AM::IMP,   R::None, R::None,  4,    1 },
    [0x01] =  { MN::LD,  AM::R_D16, R::BC,   R::None,  12,   3 },
    [0x02] =  { MN::LD,  AM::MR_R,  R::BC,   R::A,     8,    1 },

    [0x05] =  { MN::DEC, AM::R,     R::B,    R::None,  4,    1 },
    [0x06] =  { MN::LD,  AM::R_D8,  R::B,    R::None,  8,    2 },

    [0x08] =  { MN::LD,  AM::A16_R, R::None, R::SP,    20,   3 },

    [0x0A] =  { MN::LD,  AM::MR_R,  R::A,    R::BC,    8,    1 },

    [0x0E] =  { MN::LD,  AM::R_D8,  R::C,    R::None,  8,    2 },

//  0x10
    [0x11] =  { MN::LD,  AM::R_D16, R::DE,   R::None,  12,   3 },

    // 0x20
    // 0x30
    // 0x40
    // 0x50
    // 0x60
    // 0x70
    // 0x80
    // [0x80] =  { MN::ADD, AM::R_R, R::A,    R::B,    1 },
    // [0x81] =  { MN::ADD, AM::R_R, R::A,    R::C,    1 },
    // [0x82] =  { MN::ADD, AM::R_R, R::A,    R::D,    1 },
    // [0x83] =  { MN::ADD, AM::R_R, R::A,    R::E,    1 },
    // [0x84] =  { MN::ADD, AM::R_R, R::A,    R::H,    1 },
    // [0x85] =  { MN::ADD, AM::R_R, R::A,    R::L,    1 }
};

const ctx fetch(opcode_t op) { return inst[op]; }
bool exists(opcode_t op) { return &inst[op] != nullptr; }

}
}


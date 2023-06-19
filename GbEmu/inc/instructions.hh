#pragma once

#include <cstdint>
#include "common.hh"

namespace DMG01
{
namespace Instruction
{

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
    R_A16,
    None
};


typedef struct {
    Mnemonic     mnemonic;
    AddrMode     addrMode;
    Register     regA;
    Register     regB;
    std::uint8_t mCycles; // should actually be t states
    std::uint8_t len;
    std::uint8_t param;
    const char*  info;
} ctx ;


typedef Register R;
typedef AddrMode AM;
typedef Mnemonic MN;


const ctx fetch(opcode_t op);
bool      exists(opcode_t op);


const ctx inst[0x100] = {
/*   OP    :    TYPE    | ADDR MODE |  R1   |   R2    | MC | LEN */
/*  0x00 */
    [0x00] =  { MN::NOP,  AM::IMP,   R::None, R::None,  4,    1, 0, "NOP"         },
    [0x01] =  { MN::LD,   AM::R_D16, R::BC,   R::None,  12,   3, 0, "LD BC,#D16"  },
    [0x02] =  { MN::LD,   AM::MR_R,  R::BC,   R::A,     8,    1, 0, "LD BC,A"     },
    [0x03] =  { MN::INC,  AM::R,     R::BC,   R::None,  8,    1, 0, "INC BC"      },
    [0x04] =  { MN::INC,  AM::R,     R::B,    R::None,  8,    1, 0, "INC B"       },
    [0x05] =  { MN::DEC,  AM::R,     R::B,    R::None,  4,    1, 0, "DEC B"       },
    [0x06] =  { MN::LD,   AM::R_D8,  R::B,    R::None,  8,    2, 0, "LD B,#D8"    },
    [0x07] =  { MN::RLCA, AM::None,  R::None, R::None,  4,    2, 0, "RLCA"        },
    [0x08] =  { MN::LD,   AM::A16_R, R::None, R::SP,    20,   3, 0, "LD [A16],SP" },
    [0x09] =  { MN::ADD,  AM::R_R,   R::HL,   R::BC,    8,    1, 0, "ADD HL,BC"   },
    [0x0A] =  { MN::LD,   AM::R_MR,  R::A,    R::BC,    8,    1, 0, "LD A,[BC]"   },
    [0x0B] =  { MN::DEC,  AM::R,     R::BC,   R::None,  8,    1, 0, "DEC BC"      },
    [0x0C] =  { MN::INC,  AM::R,     R::C,    R::None,  4,    1, 0, "INC C"       },
    [0x0D] =  { MN::DEC,  AM::R,     R::C,    R::None,  4,    1, 0, "DEC C"       },
    [0x0E] =  { MN::LD,   AM::R_D8,  R::C,    R::None,  8,    2, 0, "LD C"        },
    [0x0F] =  { MN::RRCA, AM::None,  R::None, R::None,  4,    2, 0, "RRCA"        },

/*  0x10 */
    [0x10] =  { MN::STOP, AM::None,  R::None, R::None,  4,    2, 0, "STOP"        },
    [0x11] =  { MN::LD,   AM::R_D16, R::DE,   R::None,  12,   3, 0, "LD DE,D16"   },
    [0x12] =  { MN::LD,   AM::MR_R,  R::DE,   R::A,     12,   3, 0, "LD [DE],A"   },
    [0x13] =  { MN::INC,  AM::R,     R::DE,   R::None,  1,    8, 0, "INC DE"      },

//  0x20
    [0x20] = { MN::JR,    AM::D8,    R::None, R::None,  4,    1, 0, "NOP" },
    [0x21] = { MN::LD,    AM::R_D16, R::HL,   R::None,  4,    1, 0, "NOP" },
    [0x22] = { MN::LD,    AM::HLI_R, R::HL,   R::A,     4,    1, 0, "NOP" },
    [0x23] = { MN::INC,   AM::R,     R::HL,   R::None,  4,    1, 0, "NOP" },
    [0x24] = { MN::INC,   AM::R,     R::H,    R::None,  4,    1, 0, "NOP" },
    [0x25] = { MN::DEC,   AM::R,     R::H,    R::None,  4,    1, 0, "NOP" },
    [0x26] = { MN::LD,    AM::R_D8,  R::H,    R::None,  4,    1, 0, "NOP" },
    [0x27] = { MN::DAA },
    [0x28] = { MN::JR,    AM::D8,    R::None, R::None,  4,    1, 0, "NOP" },
    [0x29] = { MN::ADD,   AM::R_R,   R::HL,   R::HL,    4,    1, 0, "NOP" },
    [0x2A] = { MN::LD,    AM::R_HLI, R::A,    R::HL,    4,    1, 0, "NOP" },
    [0x2B] = { MN::DEC,   AM::R,     R::HL,   R::None,  4,    1, 0, "NOP" },
    [0x2C] = { MN::INC,   AM::R,     R::L,    R::None,  4,    1, 0, "NOP" },
    [0x2D] = { MN::DEC,   AM::R,     R::L,    R::None,  4,    1, 0, "NOP" },
    [0x2E] = { MN::LD,    AM::R_D8,  R::L,    R::None,  4,    1, 0, "NOP" },
    [0x2F] = { MN::CPL,   AM::None,  R::None, R::None,  4,    1, 0, "NOP"  },

//  0x30
    [0x30] = { MN::JR,    AM::D8,    R::None, R::None},
    [0x31] = { MN::LD,    AM::R_D16, R::SP,   R::None },
    [0x32] = { MN::LD,    AM::HLD_R, R::HL,   R::A},
    [0x33] = { MN::INC,   AM::R,     R::SP,   R::None},
    [0x34] = { MN::INC,   AM::MR,    R::HL,   R::None},
    [0x35] = { MN::DEC,   AM::MR,    R::HL,   R::None},
    [0x36] = { MN::LD,    AM::MR_D8, R::HL,   R::None},
    [0x37] = { MN::SCF,   AM::None,  R::None, R::None,  4,    2, 0, "RRCA"   },
    [0x38] = { MN::JR,    AM::D8,    R::None, R::None,},
    [0x39] = { MN::ADD,   AM::R_R,   R::HL,   R::SP},
    [0x3A] = { MN::LD,    AM::R_HLD, R::A,    R::HL},
    [0x3B] = { MN::DEC,   AM::R,     R::SP,   R::None},
    [0x3C] = { MN::INC,   AM::R,     R::A,    R::None},
    [0x3D] = { MN::DEC,   AM::R,     R::A,    R::None},
    [0x3E] = { MN::LD,    AM::R_D8,  R::A,    R::None},
    [0x3F] = { MN::CCF,   AM::None,  R::None, R::None,  4,    2, 0, "RRCA"  },

//  0x40
    [0x40] = { MN::LD,    AM::R_R,   R::B,    R::B},
    [0x41] = { MN::LD,    AM::R_R,   R::B,    R::C},
    [0x42] = { MN::LD,    AM::R_R,   R::B,    R::D},
    [0x43] = { MN::LD,    AM::R_R,   R::B,    R::E},
    [0x44] = { MN::LD,    AM::R_R,   R::B,    R::H},
    [0x45] = { MN::LD,    AM::R_R,   R::B,    R::L},
    [0x46] = { MN::LD,    AM::R_MR,  R::B,    R::HL},
    [0x47] = { MN::LD,    AM::R_R,   R::B,    R::A},
    [0x48] = { MN::LD,    AM::R_R,   R::C,    R::B},
    [0x49] = { MN::LD,    AM::R_R,   R::C,    R::C},
    [0x4A] = { MN::LD,    AM::R_R,   R::C,    R::D},
    [0x4B] = { MN::LD,    AM::R_R,   R::C,    R::E},
    [0x4C] = { MN::LD,    AM::R_R,   R::C,    R::H},
    [0x4D] = { MN::LD,    AM::R_R,   R::C,    R::L},
    [0x4E] = { MN::LD,    AM::R_MR,  R::C,    R::HL},
    [0x4F] = { MN::LD,    AM::R_R,   R::C,    R::A},

    // 0x50
    [0x50] = { MN::LD,    AM::R_R,   R::D,    R::B},
    [0x51] = { MN::LD,    AM::R_R,   R::D,    R::C},
    [0x52] = { MN::LD,    AM::R_R,   R::D,    R::D},
    [0x53] = { MN::LD,    AM::R_R,   R::D,    R::E},
    [0x54] = { MN::LD,    AM::R_R,   R::D,    R::H},
    [0x55] = { MN::LD,    AM::R_R,   R::D,    R::L},
    [0x56] = { MN::LD,    AM::R_MR,  R::D,    R::HL},
    [0x57] = { MN::LD,    AM::R_R,   R::D,    R::A},
    [0x58] = { MN::LD,    AM::R_R,   R::E,    R::B},
    [0x59] = { MN::LD,    AM::R_R,   R::E,    R::C},
    [0x5A] = { MN::LD,    AM::R_R,   R::E,    R::D},
    [0x5B] = { MN::LD,    AM::R_R,   R::E,    R::E},
    [0x5C] = { MN::LD,    AM::R_R,   R::E,    R::H},
    [0x5D] = { MN::LD,    AM::R_R,   R::E,    R::L},
    [0x5E] = { MN::LD,    AM::R_MR,  R::E,    R::HL},
    [0x5F] = { MN::LD,    AM::R_R,   R::E,    R::A},

//  0x60
    [0x60] = { MN::LD,    AM::R_R,   R::H,    R::B},
    [0x61] = { MN::LD,    AM::R_R,   R::H,    R::C},
    [0x62] = { MN::LD,    AM::R_R,   R::H,    R::D},
    [0x63] = { MN::LD,    AM::R_R,   R::H,    R::E},
    [0x64] = { MN::LD,    AM::R_R,   R::H,    R::H},
    [0x65] = { MN::LD,    AM::R_R,   R::H,    R::L},
    [0x66] = { MN::LD,    AM::R_MR,  R::H,    R::HL},
    [0x67] = { MN::LD,    AM::R_R,   R::H,    R::A},
    [0x68] = { MN::LD,    AM::R_R,   R::L,    R::B},
    [0x69] = { MN::LD,    AM::R_R,   R::L,    R::C},
    [0x6A] = { MN::LD,    AM::R_R,   R::L,    R::D},
    [0x6B] = { MN::LD,    AM::R_R,   R::L,    R::E},
    [0x6C] = { MN::LD,    AM::R_R,   R::L,    R::H},
    [0x6D] = { MN::LD,    AM::R_R,   R::L,    R::L},
    [0x6E] = { MN::LD,    AM::R_MR,  R::L,    R::HL},
    [0x6F] = { MN::LD,    AM::R_R,   R::L,    R::A},

//  0x70
    [0x70] = { MN::LD,    AM::MR_R,  R::HL,   R::B},
    [0x71] = { MN::LD,    AM::MR_R,  R::HL,   R::C},
    [0x72] = { MN::LD,    AM::MR_R,  R::HL,   R::D},
    [0x73] = { MN::LD,    AM::MR_R,  R::HL,   R::E},
    [0x74] = { MN::LD,    AM::MR_R,  R::HL,   R::H},
    [0x75] = { MN::LD,    AM::MR_R,  R::HL,   R::L},
    [0x76] = { MN::HALT },
    [0x77] = { MN::LD,    AM::MR_R,  R::HL,   R::A},
    [0x78] = { MN::LD,    AM::R_R,   R::A,    R::B},
    [0x79] = { MN::LD,    AM::R_R,   R::A,    R::C},
    [0x7A] = { MN::LD,    AM::R_R,   R::A,    R::D},
    [0x7B] = { MN::LD,    AM::R_R,   R::A,    R::E},
    [0x7C] = { MN::LD,    AM::R_R,   R::A,    R::H},
    [0x7D] = { MN::LD,    AM::R_R,   R::A,    R::L},
    [0x7E] = { MN::LD,    AM::R_MR,  R::A,    R::HL},
    [0x7F] = { MN::LD,    AM::R_R,   R::A,    R::A},

//  0x80
    // [0x80] =  { MN::ADD, AM::R_R, R::A,    R::B,    1 },
    // [0x81] =  { MN::ADD, AM::R_R, R::A,    R::C,    1 },
    // [0x82] =  { MN::ADD, AM::R_R, R::A,    R::D,    1 },
    // [0x83] =  { MN::ADD, AM::R_R, R::A,    R::E,    1 },
    // [0x84] =  { MN::ADD, AM::R_R, R::A,    R::H,    1 },
    // [0x85] =  { MN::ADD, AM::R_R, R::A,    R::L,    1 }
    // 0x90
    // 0xA0
    // 0xB0
    // 0xC0
    // 0xD0
    // 0xE0
    // 0xF0
};

}
}


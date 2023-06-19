#pragma once

#include <cstdint>
#include "common.hh"

namespace DMG01
{
namespace Instruction
{

enum class Mnemonic {
    None,
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
/*  OP     :    TYPE    | ADDR MODE  |   R1   |    R2   | MC | LEN */
/*  0x00 */
    [0x00] =  { MN::NOP,  AM::IMP,    R::None,  R::None,  4,    1, 0, "NOP"         },
    [0x01] =  { MN::LD,   AM::R_D16,  R::BC,    R::None,  12,   3, 0, "LD BC,#D16"  },
    [0x02] =  { MN::LD,   AM::MR_R,   R::BC,    R::A,     8,    1, 0, "LD BC,A"     },
    [0x03] =  { MN::INC,  AM::R,      R::BC,    R::None,  8,    1, 0, "INC BC"      },
    [0x04] =  { MN::INC,  AM::R,      R::B,     R::None,  8,    1, 0, "INC B"       },
    [0x05] =  { MN::DEC,  AM::R,      R::B,     R::None,  4,    1, 0, "DEC B"       },
    [0x06] =  { MN::LD,   AM::R_D8,   R::B,     R::None,  8,    2, 0, "LD B,#D8"    },
    [0x07] =  { MN::RLCA, AM::None,   R::None,  R::None,  4,    2, 0, "RLCA"        },
    [0x08] =  { MN::LD,   AM::A16_R,  R::None,  R::SP,    20,   3, 0, "LD [A16],SP" },
    [0x09] =  { MN::ADD,  AM::R_R,    R::HL,    R::BC,    8,    1, 0, "ADD HL,BC"   },
    [0x0A] =  { MN::LD,   AM::R_MR,   R::A,     R::BC,    8,    1, 0, "LD A,[BC]"   },
    [0x0B] =  { MN::DEC,  AM::R,      R::BC,    R::None,  8,    1, 0, "DEC BC"      },
    [0x0C] =  { MN::INC,  AM::R,      R::C,     R::None,  4,    1, 0, "INC C"       },
    [0x0D] =  { MN::DEC,  AM::R,      R::C,     R::None,  4,    1, 0, "DEC C"       },
    [0x0E] =  { MN::LD,   AM::R_D8,   R::C,     R::None,  8,    2, 0, "LD C"        },
    [0x0F] =  { MN::RRCA, AM::None,   R::None,  R::None,  4,    2, 0, "RRCA"        },


    //TODO: FILL OUT MC and LEN BELOW

/*  0x10 */
    [0x10] =  { MN::STOP, AM::None,   R::None,  R::None,  4,    2, 0, "STOP D8"     },
    [0x11] =  { MN::LD,   AM::R_D16,  R::DE,    R::None,  12,   3, 0, "LD DE,D16"   },
    [0x12] =  { MN::LD,   AM::MR_R,   R::DE,    R::A,     12,   3, 0, "LD [DE],A"   },
    [0x13] =  { MN::INC,  AM::R,      R::DE,    R::None,  1,    8, 0, "INC DE"      },

//  0x20
    [0x20] = { MN::JR,    AM::D8,     R::None,  R::None,  4,    1, 0, "JR NZ #D8"   },
    [0x21] = { MN::LD,    AM::R_D16,  R::HL,    R::None,  4,    1, 0, "LD HL,#D16"  },
    [0x22] = { MN::LD,    AM::HLI_R,  R::HL,    R::A,     4,    1, 0, "LD [HL+],A"  },
    [0x23] = { MN::INC,   AM::R,      R::HL,    R::None,  4,    1, 0, "INC HL"      },
    [0x24] = { MN::INC,   AM::R,      R::H,     R::None,  4,    1, 0, "INC H"       },
    [0x25] = { MN::DEC,   AM::R,      R::H,     R::None,  4,    1, 0, "DEC H"       },
    [0x26] = { MN::LD,    AM::R_D8,   R::H,     R::None,  4,    1, 0, "LD H,#D8"    },
    [0x27] = { MN::DAA,   AM::None,   R::None,  R::None,  4,    1, 0, "DAA"         },
    [0x28] = { MN::JR,    AM::D8,     R::None,  R::None,  4,    1, 0, "JR Z D8"     },
    [0x29] = { MN::ADD,   AM::R_R,    R::HL,    R::HL,    4,    1, 0, "ADD HL,HL"   },
    [0x2A] = { MN::LD,    AM::R_HLI,  R::A,     R::HL,    4,    1, 0, "LD A,[HL+]"  },
    [0x2B] = { MN::DEC,   AM::R,      R::HL,    R::None,  4,    1, 0, "DEC HL"      },
    [0x2C] = { MN::INC,   AM::R,      R::L,     R::None,  4,    1, 0, "INC L"       },
    [0x2D] = { MN::DEC,   AM::R,      R::L,     R::None,  4,    1, 0, "DEC L"       },
    [0x2E] = { MN::LD,    AM::R_D8,   R::L,     R::None,  4,    1, 0, "LD L,#D8"    },
    [0x2F] = { MN::CPL,   AM::None,   R::None,  R::None,  4,    1, 0, "CPL"         },

//  0x30
    [0x30] = { MN::JR,    AM::D8,     R::None,  R::None,  4,    1, 0, "JL NC #D8"   },
    [0x31] = { MN::LD,    AM::R_D16,  R::SP,    R::None,  4,    1, 0, "LD SP,#D16"  },
    [0x32] = { MN::LD,    AM::HLD_R,  R::HL,    R::A,     4,    1, 0, "LD [HL-],A"  },
    [0x33] = { MN::INC,   AM::R,      R::SP,    R::None,  4,    1, 0, "INC SP"      },
    [0x34] = { MN::INC,   AM::MR,     R::HL,    R::None,  4,    1, 0, "INC [HL]"    },
    [0x35] = { MN::DEC,   AM::MR,     R::HL,    R::None,  4,    1, 0, "DEC [HL]"    },
    [0x36] = { MN::LD,    AM::MR_D8,  R::HL,    R::None,  4,    1, 0, "LD [HL],#D8" },
    [0x37] = { MN::SCF,   AM::None,   R::None,  R::None,  4,    2, 0, "SCF"         },
    [0x38] = { MN::JR,    AM::D8,     R::None,  R::None,  4,    1, 0, "JR c #D8"    },
    [0x39] = { MN::ADD,   AM::R_R,    R::HL,    R::SP,    4,    1, 0, "ADD HL,SP"   },
    [0x3A] = { MN::LD,    AM::R_HLD,  R::A,     R::HL,    4,    1, 0, "LD A,[HL-]"  },
    [0x3B] = { MN::DEC,   AM::R,      R::SP,    R::None,  4,    1, 0, "DEC SP"      },
    [0x3C] = { MN::INC,   AM::R,      R::A,     R::None,  4,    1, 0, "INC A"       },
    [0x3D] = { MN::DEC,   AM::R,      R::A,     R::None,  4,    1, 0, "DEC A"       },
    [0x3E] = { MN::LD,    AM::R_D8,   R::A,     R::None,  4,    1, 0, "LD A,#D8"    },
    [0x3F] = { MN::CCF,   AM::None,   R::None,  R::None,  4,    2, 0, "CCF"         },

//  0x40
    [0x40] = { MN::LD,    AM::R_R,    R::B,     R::B,     4,    1, 0, "LD B,B"      },
    [0x41] = { MN::LD,    AM::R_R,    R::B,     R::C,     4,    1, 0, "LD B,C"      },
    [0x42] = { MN::LD,    AM::R_R,    R::B,     R::D,     4,    1, 0, "LD B,D"      },
    [0x43] = { MN::LD,    AM::R_R,    R::B,     R::E,     4,    1, 0, "LD B,E"      },
    [0x44] = { MN::LD,    AM::R_R,    R::B,     R::H,     4,    1, 0, "LD B,H"      },
    [0x45] = { MN::LD,    AM::R_R,    R::B,     R::L,     4,    1, 0, "LD B,L"      },
    [0x46] = { MN::LD,    AM::R_MR,   R::B,     R::HL,    4,    1, 0, "LD B,[HL]"   },
    [0x47] = { MN::LD,    AM::R_R,    R::B,     R::A,     4,    1, 0, "LD B,A"      },
    [0x48] = { MN::LD,    AM::R_R,    R::C,     R::B,     4,    1, 0, "LD C,B"      },
    [0x49] = { MN::LD,    AM::R_R,    R::C,     R::C,     4,    1, 0, "LD C,C"      },
    [0x4A] = { MN::LD,    AM::R_R,    R::C,     R::D,     4,    1, 0, "LD C,D"      },
    [0x4B] = { MN::LD,    AM::R_R,    R::C,     R::E,     4,    1, 0, "LD C,E"      },
    [0x4C] = { MN::LD,    AM::R_R,    R::C,     R::H,     4,    1, 0, "LD C,H"      },
    [0x4D] = { MN::LD,    AM::R_R,    R::C,     R::L,     4,    1, 0, "LD C,L"      },
    [0x4E] = { MN::LD,    AM::R_MR,   R::C,     R::HL,    4,    1, 0, "LD C,[HL]"   },
    [0x4F] = { MN::LD,    AM::R_R,    R::C,     R::A,     4,    1, 0, "LD C,A"      },

    // 0x50
    [0x50] = { MN::LD,    AM::R_R,    R::D,     R::B,     4,    1, 0, "LD D,B"      },
    [0x51] = { MN::LD,    AM::R_R,    R::D,     R::C,     4,    1, 0, "LD D,C"      },
    [0x52] = { MN::LD,    AM::R_R,    R::D,     R::D,     4,    1, 0, "LD D,D"      },
    [0x53] = { MN::LD,    AM::R_R,    R::D,     R::E,     4,    1, 0, "LD D,E"      },
    [0x54] = { MN::LD,    AM::R_R,    R::D,     R::H,     4,    1, 0, "LD D,H"      },
    [0x55] = { MN::LD,    AM::R_R,    R::D,     R::L,     4,    1, 0, "LD D,L"      },
    [0x56] = { MN::LD,    AM::R_MR,   R::D,     R::HL,    4,    1, 0, "LD D,[HL]"   },
    [0x57] = { MN::LD,    AM::R_R,    R::D,     R::A,     4,    1, 0, "LD D,A"      },
    [0x58] = { MN::LD,    AM::R_R,    R::E,     R::B,     4,    1, 0, "LD E,B"      },
    [0x59] = { MN::LD,    AM::R_R,    R::E,     R::C,     4,    1, 0, "LD E,C"      },
    [0x5A] = { MN::LD,    AM::R_R,    R::E,     R::D,     4,    1, 0, "LD E,D"      },
    [0x5B] = { MN::LD,    AM::R_R,    R::E,     R::E,     4,    1, 0, "LD E,E"      },
    [0x5C] = { MN::LD,    AM::R_R,    R::E,     R::H,     4,    1, 0, "LD E,H"      },
    [0x5D] = { MN::LD,    AM::R_R,    R::E,     R::L,     4,    1, 0, "LD E,L"      },
    [0x5E] = { MN::LD,    AM::R_MR,   R::E,     R::HL,    4,    1, 0, "LD E,[HL]"   },
    [0x5F] = { MN::LD,    AM::R_R,    R::E,     R::A,     4,    1, 0, "LD E,A"      },

//  0x60
    [0x60] = { MN::LD,    AM::R_R,    R::H,     R::B,     4,    1, 0, "LD H,B"      },
    [0x61] = { MN::LD,    AM::R_R,    R::H,     R::C,     4,    1, 0, "LD H,C"      },
    [0x62] = { MN::LD,    AM::R_R,    R::H,     R::D,     4,    1, 0, "LD H,D"      },
    [0x63] = { MN::LD,    AM::R_R,    R::H,     R::E,     4,    1, 0, "LD H,E"      },
    [0x64] = { MN::LD,    AM::R_R,    R::H,     R::H,     4,    1, 0, "LD H,H"      },
    [0x65] = { MN::LD,    AM::R_R,    R::H,     R::L,     4,    1, 0, "LD H,L"      },
    [0x66] = { MN::LD,    AM::R_MR,   R::H,     R::HL,    4,    1, 0, "LD H,[HL]"   },
    [0x67] = { MN::LD,    AM::R_R,    R::H,     R::A,     4,    1, 0, "LD H,A"      },
    [0x68] = { MN::LD,    AM::R_R,    R::L,     R::B,     4,    1, 0, "LD L,B"      },
    [0x69] = { MN::LD,    AM::R_R,    R::L,     R::C,     4,    1, 0, "LD L,C"      },
    [0x6A] = { MN::LD,    AM::R_R,    R::L,     R::D,     4,    1, 0, "LD L,D"      },
    [0x6B] = { MN::LD,    AM::R_R,    R::L,     R::E,     4,    1, 0, "LD L,E"      },
    [0x6C] = { MN::LD,    AM::R_R,    R::L,     R::H,     4,    1, 0, "LD L,H"      },
    [0x6D] = { MN::LD,    AM::R_R,    R::L,     R::L,     4,    1, 0, "LD L,L"      },
    [0x6E] = { MN::LD,    AM::R_MR,   R::L,     R::HL,    4,    1, 0, "LD L,[HL]"   },
    [0x6F] = { MN::LD,    AM::R_R,    R::L,     R::A,     4,    1, 0, "LD L,B"      },

//  0x70
    [0x70] = { MN::LD,    AM::MR_R,   R::HL,    R::B,     4,    1, 0, "LD [HL],B"   },
    [0x71] = { MN::LD,    AM::MR_R,   R::HL,    R::C,     4,    1, 0, "LD [HL],C"   },
    [0x72] = { MN::LD,    AM::MR_R,   R::HL,    R::D,     4,    1, 0, "LD [HL],D"   },
    [0x73] = { MN::LD,    AM::MR_R,   R::HL,    R::E,     4,    1, 0, "LD [HL],E"   },
    [0x74] = { MN::LD,    AM::MR_R,   R::HL,    R::H,     4,    1, 0, "LD [HL],H"   },
    [0x75] = { MN::LD,    AM::MR_R,   R::HL,    R::L,     4,    1, 0, "LD [HL],L"   },
    [0x76] = { MN::HALT,  AM::None,   R::None,  R::None,  4,    2, 0, "RRCA"        },
    [0x77] = { MN::LD,    AM::MR_R,   R::HL,    R::A,     4,    1, 0, "LD [HL],A"   },
    [0x78] = { MN::LD,    AM::R_R,    R::A,     R::B,     4,    1, 0, "LD A,B"      },
    [0x79] = { MN::LD,    AM::R_R,    R::A,     R::C,     4,    1, 0, "LD A,C"      },
    [0x7A] = { MN::LD,    AM::R_R,    R::A,     R::D,     4,    1, 0, "LD A,D"      },
    [0x7B] = { MN::LD,    AM::R_R,    R::A,     R::E,     4,    1, 0, "LD A,E"      },
    [0x7C] = { MN::LD,    AM::R_R,    R::A,     R::H,     4,    1, 0, "LD A,H"      },
    [0x7D] = { MN::LD,    AM::R_R,    R::A,     R::L,     4,    1, 0, "LD A,L"      },
    [0x7E] = { MN::LD,    AM::R_MR,   R::A,     R::HL,    4,    1, 0, "LD A,[HL]"   },
    [0x7F] = { MN::LD,    AM::R_R,    R::A,     R::A,     4,    1, 0, "LD A,A"      },

 //0x8X
    [0x80] = { MN::ADD,   AM::R_R,    R::A,     R::B,     4,    1, 0, "ADD A,B"     },
    [0x81] = { MN::ADD,   AM::R_R,    R::A,     R::C,     4,    1, 0, "ADD A,C"     },
    [0x82] = { MN::ADD,   AM::R_R,    R::A,     R::D,     4,    1, 0, "ADD A,D"     },
    [0x83] = { MN::ADD,   AM::R_R,    R::A,     R::E,     4,    1, 0, "ADD A,E"     },
    [0x84] = { MN::ADD,   AM::R_R,    R::A,     R::H,     4,    1, 0, "ADD A,H"     },
    [0x85] = { MN::ADD,   AM::R_R,    R::A,     R::L,     4,    1, 0, "ADD A,L"     },
    [0x86] = { MN::ADD,   AM::R_MR,   R::A,     R::HL,    4,    1, 0, "ADD A,[HL]"  },
    [0x87] = { MN::ADD,   AM::R_R,    R::A,     R::A,     4,    1, 0, "ADD A,A"     },
    [0x88] = { MN::ADC,   AM::R_R,    R::A,     R::B,     4,    1, 0, "ADC A,B"     },
    [0x89] = { MN::ADC,   AM::R_R,    R::A,     R::C,     4,    1, 0, "ADC A,C"     },
    [0x8A] = { MN::ADC,   AM::R_R,    R::A,     R::D,     4,    1, 0, "ADC A,D"     },
    [0x8B] = { MN::ADC,   AM::R_R,    R::A,     R::E,     4,    1, 0, "ADC A,E"     },
    [0x8C] = { MN::ADC,   AM::R_R,    R::A,     R::H,     4,    1, 0, "ADC A,H"     },
    [0x8D] = { MN::ADC,   AM::R_R,    R::A,     R::L,     4,    1, 0, "ADC A,L"     },
    [0x8E] = { MN::ADC,   AM::R_MR,   R::A,     R::HL,    4,    1, 0, "ADC A,[HL]"  },
    [0x8F] = { MN::ADC,   AM::R_R,    R::A,     R::A,     4,    1, 0, "ADC A,A"     },

    //0x9X
    [0x90] = { MN::SUB,   AM::R_R,    R::A,     R::B},
    [0x91] = { MN::SUB,   AM::R_R,    R::A,     R::C},
    [0x92] = { MN::SUB,   AM::R_R,    R::A,     R::D},
    [0x93] = { MN::SUB,   AM::R_R,    R::A,     R::E},
    [0x94] = { MN::SUB,   AM::R_R,    R::A,     R::H},
    [0x95] = { MN::SUB,   AM::R_R,    R::A,     R::L},
    [0x96] = { MN::SUB,   AM::R_MR,   R::A,     R::HL},
    [0x97] = { MN::SUB,   AM::R_R,    R::A,     R::A},
    [0x98] = { MN::SBC,   AM::R_R,    R::A,     R::B},
    [0x99] = { MN::SBC,   AM::R_R,    R::A,     R::C},
    [0x9A] = { MN::SBC,   AM::R_R,    R::A,     R::D},
    [0x9B] = { MN::SBC,   AM::R_R,    R::A,     R::E},
    [0x9C] = { MN::SBC,   AM::R_R,    R::A,     R::H},
    [0x9D] = { MN::SBC,   AM::R_R,    R::A,     R::L},
    [0x9E] = { MN::SBC,   AM::R_MR,   R::A,     R::HL},
    [0x9F] = { MN::SBC,   AM::R_R,    R::A,     R::A},


    //0xAX
    [0xA0] = { MN::AND,   AM::R_R,    R::A,     R::B},
    [0xA1] = { MN::AND,   AM::R_R,    R::A,     R::C},
    [0xA2] = { MN::AND,   AM::R_R,    R::A,     R::D},
    [0xA3] = { MN::AND,   AM::R_R,    R::A,     R::E},
    [0xA4] = { MN::AND,   AM::R_R,    R::A,     R::H},
    [0xA5] = { MN::AND,   AM::R_R,    R::A,     R::L},
    [0xA6] = { MN::AND,   AM::R_MR,   R::A,     R::HL},
    [0xA7] = { MN::AND,   AM::R_R,    R::A,     R::A},
    [0xA8] = { MN::XOR,   AM::R_R,    R::A,     R::B},
    [0xA9] = { MN::XOR,   AM::R_R,    R::A,     R::C},
    [0xAA] = { MN::XOR,   AM::R_R,    R::A,     R::D},
    [0xAB] = { MN::XOR,   AM::R_R,    R::A,     R::E},
    [0xAC] = { MN::XOR,   AM::R_R,    R::A,     R::H},
    [0xAD] = { MN::XOR,   AM::R_R,    R::A,     R::L},
    [0xAE] = { MN::XOR,   AM::R_MR,   R::A,     R::HL},
    [0xAF] = { MN::XOR,   AM::R_R,    R::A,     R::A},

    //0xBX
    [0xB0] = { MN::OR,    AM::R_R,    R::A,     R::B},
    [0xB1] = { MN::OR,    AM::R_R,    R::A,     R::C},
    [0xB2] = { MN::OR,    AM::R_R,    R::A,     R::D},
    [0xB3] = { MN::OR,    AM::R_R,    R::A,     R::E},
    [0xB4] = { MN::OR,    AM::R_R,    R::A,     R::H},
    [0xB5] = { MN::OR,    AM::R_R,    R::A,     R::L},
    [0xB6] = { MN::OR,    AM::R_MR,   R::A,     R::HL},
    [0xB7] = { MN::OR,    AM::R_R,    R::A,     R::A},
    [0xB8] = { MN::CP,    AM::R_R,    R::A,     R::B},
    [0xB9] = { MN::CP,    AM::R_R,    R::A,     R::C},
    [0xBA] = { MN::CP,    AM::R_R,    R::A,     R::D},
    [0xBB] = { MN::CP,    AM::R_R,    R::A,     R::E},
    [0xBC] = { MN::CP,    AM::R_R,    R::A,     R::H},
    [0xBD] = { MN::CP,    AM::R_R,    R::A,     R::L},
    [0xBE] = { MN::CP,    AM::R_MR,   R::A,     R::HL},
    [0xBF] = { MN::CP,    AM::R_R,    R::A,     R::A},

    [0xC0] = { MN::RET,   AM::IMP,    R::None,  R::None },
    [0xC1] = { MN::POP,   AM::R,      R::BC,    R::None },
    [0xC2] = { MN::JP,    AM::D16,    R::None,  R::None },
    [0xC3] = { MN::JP,    AM::D16,    R::None,  R::None },
    [0xC4] = { MN::CALL,  AM::D16,    R::None,  R::None },
    [0xC5] = { MN::PUSH,  AM::R,      R::BC,    R::None },
    [0xC6] = { MN::ADD,   AM::R_D8,   R::A,     R::None },
    [0xC7] = { MN::RST,   AM::IMP,    R::None,  R::None },
    [0xC8] = { MN::RET,   AM::IMP,    R::None,  R::None },
    [0xC9] = { MN::RET,   AM::None,   R::None,  R::None,  4,    2, 0, "RET"  },
    [0xCA] = { MN::JP,    AM::D16,    R::None,  R::None },
    [0xCB] = { MN::CB,    AM::D8,     R::None,  R::None,  4,    2, 0, "CB" },
    [0xCC] = { MN::CALL,  AM::D16,    R::None,  R::None },
    [0xCD] = { MN::CALL,  AM::D16,    R::None,  R::None,  4,    2, 0, "CALL " },
    [0xCE] = { MN::ADC,   AM::R_D8,   R::A,     R::None },
    [0xCF] = { MN::RST,   AM::IMP,    R::None,  R::None },

    [0xD0] = { MN::RET,   AM::IMP,    R::None,  R::None },
    [0xD1] = { MN::POP,   AM::R,      R::DE,    R::None },
    [0xD2] = { MN::JP,    AM::D16,    R::None,  R::None },
    [0xD4] = { MN::CALL,  AM::D16,    R::None,  R::None },
    [0xD5] = { MN::PUSH,  AM::R,      R::DE,    R::None },
    [0xD6] = { MN::SUB,   AM::R_D8,   R::A,     R::None },
    [0xD7] = { MN::RST,   AM::IMP,    R::None,  R::None },
    [0xD8] = { MN::RET,   AM::IMP,    R::None,  R::None },
    [0xD9] = { MN::RETI,  AM::None,   R::None,  R::None,  4,    2, 0, "RRCA"  },
    [0xDA] = { MN::JP,    AM::D16,    R::None,  R::None },
    [0xDC] = { MN::CALL,  AM::D16,    R::None,  R::None },
    [0xDE] = { MN::SBC,   AM::R_D8,   R::A,     R::None },
    [0xDF] = { MN::RST,   AM::IMP,    R::None,  R::None },

    //0xEX
    [0xE0] = { MN::LDH,   AM::A8_R,   R::None,  R::A    },
    [0xE1] = { MN::POP,   AM::R,      R::HL,    R::None },
    [0xE2] = { MN::LD,    AM::MR_R,   R::C,     R::A    },
    [0xE5] = { MN::PUSH,  AM::R,      R::HL,    R::None },
    [0xE6] = { MN::AND,   AM::R_D8,   R::A,     R::None },
    [0xE7] = { MN::RST,   AM::IMP,    R::None,  R::None },
    [0xE8] = { MN::ADD,   AM::R_D8,   R::SP,    R::None },
    [0xE9] = { MN::JP,    AM::R,      R::HL,    R::None },
    [0xEA] = { MN::LD,    AM::A16_R,  R::None,  R::A    },
    [0xEE] = { MN::XOR,   AM::R_D8,   R::A,     R::None },
    [0xEF] = { MN::RST,   AM::IMP,    R::None,  R::None },


    //0xFX
    [0xF0] = { MN::LDH,   AM::R_A8,   R::A,     R::None },
    [0xF1] = { MN::POP,   AM::R,      R::AF,    R::None },
    [0xF2] = { MN::LD,    AM::R_MR,   R::A,     R::C    },
    [0xF3] = { MN::DI,    AM::None,   R::None,  R::None,  4,    2, 0, "RRCA"  },
    [0xF5] = { MN::PUSH,  AM::R,      R::AF,    R::None },
    [0xF6] = { MN::OR,    AM::R_D8,   R::A,     R::None },
    [0xF7] = { MN::RST,   AM::IMP,    R::None,  R::None },
    [0xF8] = { MN::LD,    AM::HL_SPR, R::HL,    R::SP   },
    [0xF9] = { MN::LD,    AM::R_R,    R::SP,    R::HL   },
    [0xFA] = { MN::LD,    AM::R_A16,  R::A,     R::None },
    [0xFB] = { MN::EI,    AM::None,   R::None,  R::None,  4,    2, 0, "RRCA"  },
    [0xFE] = { MN::CP,    AM::R_D8,   R::A,     R::None },
    [0xFF] = { MN::RST,   AM::IMP,    R::None,  R::None },
};

}
}


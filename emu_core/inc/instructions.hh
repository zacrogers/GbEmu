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
    R_D16,
    R_R,
    MR_R,
    R,
    R_D8,
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

}
}


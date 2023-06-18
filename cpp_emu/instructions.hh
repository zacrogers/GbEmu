#pragma once

#include <cstdint>
#include "common.hh"

namespace DMG01
{
namespace Instruction{

enum class Mnemonic
{
    NOP,
    ADD, ADDHL, ADC, SUB, SBC,

    AND, OR, XOR,

    CP, INC, DEC, CCF, SCF,

    RRA, RLA, RRCA, RRLA,

    CPL, BIT, RESET, SET, SRL, RR, RL, RRC, RLC, SRA, SLA, SWAP, count
};

enum class AddrMode {
    IMP
};

typedef Register R;
typedef AddrMode AM;
typedef Mnemonic MN;


typedef struct {
    MN           mnemonic;
    AM           addrMode;
    R            regA;
    R            regB;
    std::uint8_t mCycles;
} ctx ;

static const ctx inst[0x100] = {
    [0x00] =  { MN::NOP, AM::IMP, R::None, R::None, 1 },
    [0x80] =  { MN::ADD, AM::IMP, R::A,    R::B,    1 },
    [0x81] =  { MN::ADD, AM::IMP, R::A,    R::C,    1 },
    [0x82] =  { MN::ADD, AM::IMP, R::A,    R::D,    1 },
    [0x83] =  { MN::ADD, AM::IMP, R::A,    R::E,    1 },
    [0x84] =  { MN::ADD, AM::IMP, R::A,    R::H,    1 },
    [0x85] =  { MN::ADD, AM::IMP, R::A,    R::L,    1 }
};

const ctx* fetch(opcode_t op) { return &inst[op]; }
bool exists(opcode_t op) {return true;}

}
}


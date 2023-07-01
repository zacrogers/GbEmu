#include "../inc/process.hh"

#include "../inc/cpu.hh"

#include "etl/map.h"


void kingOfTheCastle(DMG01::Cpu* cpu)
{
    cpu->bus()->read16(0xFF);
}


namespace DMG01
{
namespace Process
{

void stackPush8(Registers *reg, Bus *bus, std::uint8_t val)
{
    bus->write(reg->spDecrement(), val);
}


void stackPush16(Registers *reg, Bus *bus, std::uint16_t val)
{
    bus->write(reg->spDecrement(), (val >> 8) & 0xFF);
    bus->write(reg->spDecrement(), val & 0xFF);
}

std::uint8_t stackPop8(Registers *reg, Bus *bus)
{
    return bus->read(reg->spIncrement());
}


std::uint16_t stackPop16(Registers *reg, Bus *bus)
{
    auto hi = bus->read(reg->spIncrement());
    auto lo = bus->read(reg->spIncrement());

    return (hi << 8) | lo;
}

static void gotoAddress(Registers *reg, Bus *bus, address_t addr, bool pushPc)
{
    // if (check_cond(ctx)) {
        if (pushPc)
        {
            // emu_cycles(2);
            stackPush16(reg, bus, reg->pcGet());
        }

        reg->pcSet(addr);
        // emu_cycles(1);
    // }
}



static void nop(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void ld(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    if(reg->destIsMem)         //LD (BC), A for instance...
    {
        reg->write(inst->regB, reg->getOpA());
        return;
    }

}


static void inc(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    std::uint16_t result = reg->getOpA() + 1;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    // printf("REG A: %D, 0x%04x\n\n", static_cast<int>(inst->regA), result);
    reg->write(inst->regA, result);
}


static void dec(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    std::uint16_t result = reg->getOpA() - 1;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    // printf("REG A: %D, 0x%04x\n\n", static_cast<int>(inst->regA), result);
    reg->write(inst->regA, result);
}


static void rlca(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    uint8_t u = reg->read(Register::A);
    bool c = (u >> 7) & 1;

    u = (u << 1) | c;
    reg->write(Register::A, u);

    flags->zero      = 0;
    flags->subtract  = 0;
    flags->halfCarry = 0;
    flags->carry     = c;

    reg->setFlags(flags);
}


static void add(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    std::uint16_t result = reg->getOpA() + reg->getOpB();

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->write(inst->regA, static_cast<std::uint8_t>(result & 0xFF));
}


static void rrca(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    uint8_t val = reg->read(Register::A);
    uint8_t b = val & 1;

    reg->write(Register::A, val >>= 1);
    reg->write(Register::A, val |= (b << 7));

    flags->zero      = 0;
    flags->subtract  = 0;
    flags->halfCarry = 0;
    flags->carry     = b;

    reg->setFlags(flags);
}


static void stop(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rla(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void jr(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rra(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void daa(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void cpl(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void scf(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    flags->zero      = -1;
    flags->subtract  = 0;
    flags->halfCarry = 0;
    flags->carry     = 1;

    reg->setFlags(flags);
}


static void ccf(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void halt(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    // cpu->setHalted(true);
}


static void adc(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void sub(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->read(inst->regA));
    auto opB = static_cast<std::uint16_t>(reg->read(inst->regB));

    std::uint16_t result = opA - opB;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->write(inst->regA, static_cast<std::uint8_t>(result & 0xFF));
}


static void sbc(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void _and(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    reg->write(inst->regA, reg->read(inst->regA) & reg->read(inst->regB));

    flags->zero      = 0;
    flags->subtract  = 0;
    flags->halfCarry = 1;
    flags->carry     = 0;

    reg->setFlags(flags);
}


static void _xor(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    reg->write(inst->regA, reg->read(inst->regA) ^ reg->read(inst->regB));

    flags->zero      = 0;
    flags->subtract  = 0;
    flags->halfCarry = 0;
    flags->carry     = 0;

    reg->setFlags(flags);
}


static void _or(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    reg->write(inst->regA, reg->read(inst->regA) | reg->read(inst->regB));

    flags->zero      = 0;
    flags->subtract  = 0;
    flags->halfCarry = 0;
    flags->carry     = 0;

    reg->setFlags(flags);
}


static void cp(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void pop(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    auto val = stackPop16(reg, bus);
    reg->write(inst->regA, val);

    if (inst->regA == Register::AF)
    {
        reg->write(inst->regA, val & 0xFFF0);
    }
}


static void jp(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    gotoAddress(reg, bus, (address_t)reg->getOpA(), false);
}


static void push(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void ret(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    // if (ctx->cur_inst->cond != CT_NONE)
    // {
    //     emu_cycles(1);
    // }

    // if (check_cond(ctx)) {
        // u16 lo = stack_pop();
        // emu_cycles(1);
        // u16 hi = stack_pop();
        // emu_cycles(1);

        // u16 n = (hi << 8) | lo;
        reg->pcSet(stackPop16(reg, bus));
        // ctx->regs.pc = n;

        // emu_cycles(1);
    // }
}


static void cb(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void call(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void reti(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
    // ctx->int_master_enabled = true;

    ret(inst, reg, bus, flags);
}


static void ldh(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void jphl(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void di(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void ei(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rst(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void err(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}

//CB instructions...


static void rlc(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rrc(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rl(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void rr(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void sla(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void sra(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void swap(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void srl(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void bit(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void res(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


static void set(const Instruction::ctx *inst, Registers *reg, Bus *bus, Flags *flags)
{
}


const etl::map<Instruction::MN, callback, 346> callbacks = {
    {Instruction::MN::NOP,   nop},
    {Instruction::MN::LD,    ld},
    {Instruction::MN::INC,   inc},
    {Instruction::MN::DEC,   dec},
    {Instruction::MN::RLCA,  rlca},
    {Instruction::MN::ADD,   add},
    {Instruction::MN::RRCA,  rrca},
    {Instruction::MN::STOP,  stop},
    {Instruction::MN::RLA,   rla},
    {Instruction::MN::JR,    jr},
    {Instruction::MN::RRA,   rra},
    {Instruction::MN::DAA,   daa},
    {Instruction::MN::CPL,   cpl},
    {Instruction::MN::SCF,   scf},
    {Instruction::MN::CCF,   ccf},
    {Instruction::MN::HALT,  halt},
    {Instruction::MN::ADC,   adc},
    {Instruction::MN::SUB,   sub},
    {Instruction::MN::SBC,   sbc},
    {Instruction::MN::AND,   _and},
    {Instruction::MN::XOR,   _xor},
    {Instruction::MN::OR,    _or},
    {Instruction::MN::CP,    cp},
    {Instruction::MN::POP,   pop},
    {Instruction::MN::JP,    jp},
    {Instruction::MN::PUSH,  push},
    {Instruction::MN::RET,   ret},
    {Instruction::MN::CB,    cb},
    {Instruction::MN::CALL,  call},
    {Instruction::MN::RETI,  reti},
    {Instruction::MN::LDH,   ldh},
    {Instruction::MN::JPHL,  jphl},
    {Instruction::MN::DI,    di},
    {Instruction::MN::EI,    ei},
    {Instruction::MN::RST,   rst},
    {Instruction::MN::ERR,   err},
    //CB instructions...
    {Instruction::MN::RLC,   rlc},
    {Instruction::MN::RRC,   rrc},
    {Instruction::MN::RL,    rl},
    {Instruction::MN::RR,    rr},
    {Instruction::MN::SLA,   sla},
    {Instruction::MN::SRA,   sra},
    {Instruction::MN::SWAP,  swap},
    {Instruction::MN::SRL,   srl},
    {Instruction::MN::BIT,   bit},
    {Instruction::MN::RES,   res},
    {Instruction::MN::SET,   set}
};

callback get(Instruction::MN mnemonic)
{
    auto it = callbacks.find(mnemonic);
    return it->second;
}

};
}
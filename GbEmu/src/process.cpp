#include "../inc/process.hh"


namespace DMG01
{
namespace Process
{


static void nop(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void ld(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void inc(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->get8(inst->regA));

    std::uint16_t result = ++opA;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->set8(inst->regA, static_cast<std::uint8_t>(result & 0xFF00));
}


static void dec(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->get8(inst->regA));

    std::uint16_t result = --opA;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->set8(inst->regA, static_cast<std::uint8_t>(result & 0xFF00));
}


static void rcla(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void add(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->get8(inst->regA));
    auto opB = static_cast<std::uint16_t>(reg->get8(inst->regB));

    std::uint16_t result = opA + opB;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->set8(inst->regA, static_cast<std::uint8_t>(result & 0xFF00));
}


static void rrca(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void stop(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rla(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void jr(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rra(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void daa(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void cpl(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void scf(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void ccf(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void halt(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void adc(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void sub(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->get8(inst->regA));
    auto opB = static_cast<std::uint16_t>(reg->get8(inst->regB));

    std::uint16_t result = opA - opB;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->set8(inst->regA, static_cast<std::uint8_t>(result & 0xFF00));
}


static void sbc(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void _and(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void _xor(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void _or(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void cp(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void pop(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void jp(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void push(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void ret(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void cb(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void call(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void reti(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void ldh(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void jphl(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void di(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void ei(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rst(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void err(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}

//CB instructions...


static void rlc(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rrc(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rl(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void rr(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void sla(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void sra(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void swap(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void srl(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void bit(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void res(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


static void set(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


const std::map<Instruction::MN, callback> callbacks = {
    {Instruction::MN::NOP,   nop},
    {Instruction::MN::LD,    ld},
    {Instruction::MN::INC,   inc},
    {Instruction::MN::DEC,   dec},
    {Instruction::MN::RLCA,  rcla},
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
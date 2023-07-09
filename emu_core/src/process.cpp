#include "../inc/process.hh"
#include "etl/map.h"


namespace DMG01
{
namespace Process
{

void stackPush8(Cpu* cpu, std::uint8_t val)
{
    cpu->bus()->write(cpu->reg()->spDecrement(), val);
}


void stackPush16(Cpu* cpu, std::uint16_t val)
{
    cpu->bus()->write(cpu->reg()->spDecrement(), (val >> 8) & 0xFF);
    cpu->bus()->write(cpu->reg()->spDecrement(), val & 0xFF);
}

std::uint8_t stackPop8(Cpu* cpu)
{
    return cpu->bus()->read(cpu->reg()->spIncrement());
}


std::uint16_t stackPop16(Cpu* cpu)
{
    auto hi = cpu->bus()->read(cpu->reg()->spIncrement());
    auto lo = cpu->bus()->read(cpu->reg()->spIncrement());

    return (hi << 8) | lo;
}

bool check_cond(Cpu *cpu)
{
    // if(cpu->currInst)
    return true;
}

static void gotoAddress(Cpu* cpu, address_t addr, bool pushPc)
{
    if (check_cond(cpu))
    {
        if (pushPc)
        {
            // emu_cycles(2);
            stackPush16(cpu, cpu->reg()->pcGet());
        }

        cpu->reg()->pcSet(addr);
        // emu_cycles(1);
    }
}



static void nop(Cpu* cpu)
{
}


static void ld(Cpu* cpu)
{
    if(cpu->reg()->destIsMem)         //LD (BC), A for instance...
    {
        cpu->bus()->write(cpu->reg()->memDest, cpu->reg()->getOpA());
        return;
    }
    auto result = cpu->reg()->getOpB();

    if(cpu->reg()->is16(cpu->currInst.regA))
    {
        cpu->reg()->write(cpu->currInst.regA , ((result >> 8) &0xFF) | ((result & 0xFF) << 8));
    }
    else
    {
        cpu->reg()->write(cpu->currInst.regA , result);
    }

}


static void inc(Cpu* cpu)
{
    std::uint16_t result = cpu->reg()->getOpA() + 1;
    cpu->reg()->write(cpu->currInst.regA, result);

    cpu->pFlags->carry     = true;
    cpu->pFlags->halfCarry = false;
    cpu->pFlags->zero      = (result == 0);
    cpu->pFlags->subtract  = false;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void dec(Cpu* cpu)
{
    std::uint16_t result = cpu->reg()->getOpA() - 1;

    cpu->pFlags->zero  = (result == 0 )   ? true : false;
    cpu->pFlags->carry = (result > 0xFF ) ? true : false;

    cpu->reg()->setFlags(cpu->pFlags);

    // printf("REG A: %D, 0x%04x\n\n", static_cast<int>(cpu->currInst.regA), result);
    cpu->reg()->write(cpu->currInst.regA, result);
}


static void rlca(Cpu* cpu)
{
    uint8_t u = cpu->reg()->read(Register::A);
    bool c = (u >> 7) & 1;

    u = (u << 1) | c;
    cpu->reg()->write(Register::A, u);

    cpu->pFlags->zero      = 0;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 0;
    cpu->pFlags->carry     = c;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void add(Cpu* cpu)
{
    std::uint16_t result = cpu->reg()->getOpA() + cpu->reg()->getOpB();

    cpu->pFlags->zero  = (result == 0 )   ? true : false;
    cpu->pFlags->carry = (result > 0xFF ) ? true : false;

    cpu->reg()->setFlags(cpu->pFlags);

    cpu->reg()->write(cpu->currInst.regA, static_cast<std::uint8_t>(result & 0xFF));
}


static void rrca(Cpu* cpu)
{
    uint8_t val = cpu->reg()->read(Register::A);
    uint8_t b = val & 1;

    cpu->reg()->write(Register::A, val >>= 1);
    cpu->reg()->write(Register::A, val |= (b << 7));

    cpu->pFlags->zero      = 0;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 0;
    cpu->pFlags->carry     = b;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void stop(Cpu* cpu)
{
}


static void rla(Cpu* cpu)
{
}

int tosigned(unsigned x)
{
    if (x <= INT_MAX)
        return static_cast<int>(x);

    if (x >= INT_MIN)
        return static_cast<int>(x - INT_MIN) + INT_MIN;

    throw x; // Or whatever else you like
}
static void jr(Cpu* cpu)
{
    gotoAddress(cpu, cpu->reg()->pcGet() + static_cast<std::int8_t>((cpu->reg()->getOpA())), false);
}


static void rra(Cpu* cpu)
{
}


static void daa(Cpu* cpu)
{
}


static void cpl(Cpu* cpu)
{
}


static void scf(Cpu* cpu)
{
    cpu->pFlags->zero      = -1;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 0;
    cpu->pFlags->carry     = 1;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void ccf(Cpu* cpu)
{
}


static void halt(Cpu* cpu)
{
    // cpu->setHalted(true);
}


static void adc(Cpu* cpu)
{
}


static void sub(Cpu* cpu)
{
    auto opA = static_cast<std::uint16_t>(cpu->reg()->read(cpu->currInst.regA));
    auto opB = static_cast<std::uint16_t>(cpu->reg()->read(cpu->currInst.regB));

    std::uint16_t result = opA - opB;

    cpu->pFlags->zero  = (result == 0 )   ? true : false;
    cpu->pFlags->carry = (result > 0xFF ) ? true : false;

    cpu->reg()->setFlags(cpu->pFlags);

    cpu->reg()->write(cpu->currInst.regA, static_cast<std::uint8_t>(result & 0xFF));
}


static void sbc(Cpu* cpu)
{
}


static void _and(Cpu* cpu)
{
    cpu->reg()->write(cpu->currInst.regA, cpu->reg()->read(cpu->currInst.regA) & cpu->reg()->read(cpu->currInst.regB));

    cpu->pFlags->zero      = 0;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 1;
    cpu->pFlags->carry     = 0;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void _xor(Cpu* cpu)
{
    cpu->reg()->write(cpu->currInst.regA, cpu->reg()->read(cpu->currInst.regA) ^ cpu->reg()->read(cpu->currInst.regB));

    cpu->pFlags->zero      = 0;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 0;
    cpu->pFlags->carry     = 0;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void _or(Cpu* cpu)
{
    cpu->reg()->write(cpu->currInst.regA, cpu->reg()->read(cpu->currInst.regA) | cpu->reg()->read(cpu->currInst.regB));

    cpu->pFlags->zero      = 0;
    cpu->pFlags->subtract  = 0;
    cpu->pFlags->halfCarry = 0;
    cpu->pFlags->carry     = 0;

    cpu->reg()->setFlags(cpu->pFlags);
}


static void cp(Cpu* cpu)
{
}


static void pop(Cpu* cpu)
{
    auto val = stackPop16(cpu);
    cpu->reg()->write(cpu->currInst.regA, val);

    if (cpu->currInst.regA == Register::AF)
    {
        cpu->reg()->write(cpu->currInst.regA, val & 0xFFF0);
    }
}


static void jp(Cpu* cpu)
{
    gotoAddress(cpu, (address_t)cpu->reg()->getOpA(), false);
}


static void push(Cpu* cpu)
{
}


static void ret(Cpu* cpu)
{
    // if (ctx->cur_cpu->currInst.cond != CT_NONE)
    // {
    //     emu_cycles(1);
    // }

    // if (check_cond(ctx)) {
        // u16 lo = stack_pop();
        // emu_cycles(1);
        // u16 hi = stack_pop();
        // emu_cycles(1);

        // u16 n = (hi << 8) | lo;
        cpu->reg()->pcSet(stackPop16(cpu));
        // ctx->regs.pc = n;

        // emu_cycles(1);
    // }
}


static void cb(Cpu* cpu)
{
}


static void call(Cpu* cpu)
{
}


static void reti(Cpu* cpu)
{
    cpu->intrEnabled = true;
    ret(cpu);
}


static void ldh(Cpu* cpu)
{
}


static void jphl(Cpu* cpu)
{
}


static void di(Cpu* cpu)
{
    cpu->intrEnabled = false;
}


static void ei(Cpu* cpu)
{
}


static void rst(Cpu* cpu)
{
}


static void err(Cpu* cpu)
{
}

//CB instructions...


static void rlc(Cpu* cpu)
{
}


static void rrc(Cpu* cpu)
{
}


static void rl(Cpu* cpu)
{
}


static void rr(Cpu* cpu)
{
}


static void sla(Cpu* cpu)
{
}


static void sra(Cpu* cpu)
{
}


static void swap(Cpu* cpu)
{
}


static void srl(Cpu* cpu)
{
}


static void bit(Cpu* cpu)
{
}


static void res(Cpu* cpu)
{
}


static void set(Cpu* cpu)
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
#include "../inc/process.hh"


namespace DMG01
{
namespace Process
{


void nop(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
}


void add(const Instruction::ctx *inst, Registers *reg, Flags *flags)
{
    auto opA = static_cast<std::uint16_t>(reg->get8(inst->regA));
    auto opB = static_cast<std::uint16_t>(reg->get8(inst->regB));

    std::uint16_t result = opA + opB;

    flags->zero  = (result == 0 )   ? true : false;
    flags->carry = (result > 0xFF ) ? true : false;

    reg->setFlags(flags);

    reg->set8(inst->regA, static_cast<std::uint8_t>(result & 0xFF00));
}


const std::map<Instruction::MN, callback> callbacks = {
    {Instruction::MN::NOP, nop},
    {Instruction::MN::ADD, add}
};

callback get(Instruction::MN mnemonic)
{
    return callbacks.at(mnemonic);
}

};
}
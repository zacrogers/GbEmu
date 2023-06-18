#pragma once

#include <cstdint>
#include <map>
#include <functional>
#include <string>

#include "common.hh"
#include "logging.hh"
#include "instructions.hh"
#include "bus.hh"
#include "registers.hh"

namespace DMG01
{
namespace Process
{


static void nop(const Instruction::ctx *inst, Registers *reg, Flags *flags)
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


typedef std::function<void(const Instruction::ctx*, Registers*, Flags*)> callback;

const std::map<Instruction::MN, callback> callbacks = {
    {Instruction::MN::NOP, nop},
    {Instruction::MN::ADD, add}
};

callback get(Instruction::MN mnemonic)
{
    return callbacks.at(mnemonic);
}

};


class Cpu
{
public:
    Cpu()
    {}


    address_t execute()
    {
        static auto process = Process::get(currInst.mnemonic);
        process(&currInst, &reg, &f);
        return 0;
    }


    void fetchData()
    {
        switch (currInst.addrMode)
        {
        case Instruction::AM::IMP:
        case Instruction::AM::R:
            dataFetched = reg.get8(currInst.regA);
            break;

        case Instruction::AM::R_D8:
            dataFetched = *bus.read(reg.pcGet());
            // emu_cycles(1);
            reg.pcIncr();
            break;

        case Instruction::AM::R_D16:
            auto lo = static_cast<std::uint16_t>(*bus.read(reg.pcGet()));
            // emu_cycles(1);

            auto hi = static_cast<std::uint16_t>(*bus.read(reg.pcGet() + 1));
            // emu_cycles(1);

            dataFetched = lo | (hi << 8);

            reg.pcIncr();
            reg.pcIncr();
            break;

        default:
            logging::log(logging::Level::FUCK, "Unknown addressing mode: %d", static_cast<int>(currInst.addrMode));
            break;
        }
    }


    void step()
    {
        auto newOpcode = static_cast<opcode_t>(*bus.read(reg.pcIncr()));

        if(Instruction::exists(newOpcode))
        {
            currOpcode = newOpcode;
            currInst   = Instruction::fetch(currOpcode);

            fetchData();
            execute();
        }
        else
        {
            logging::log(logging::Level::FUCK, "Unknown opcode: %d", newOpcode);
        }
    }

private:
    Registers         reg         { };
    Bus               bus         { };
    Flags             f           { };    // flags
    Instruction::ctx  currInst    { };
    opcode_t          currOpcode  { 0 };
    std::uint16_t     dataFetched { 0 };
};

}
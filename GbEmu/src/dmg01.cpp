#include "../inc/dmg01.hh"

#include <stdio.h>

namespace DMG01
{

bool Cpu::init()
{
    return true;
}


address_t Cpu::execute()
{
    auto process = Process::get(currInst.mnemonic);
    if(process != nullptr)
    {
        process(&currInst, pReg, &flags);
    }
    return 0;
}


void Cpu::fetchData()
{
    switch (currInst.addrMode)
    {
    case Instruction::AM::IMP:
    case Instruction::AM::R:
        dataFetched = pReg->get8(currInst.regA);
        break;

    case Instruction::AM::R_D8:
        dataFetched = pBus->read(pReg->pcGet());
        // emu_cycles(1);
        pReg->pcIncr();
        break;

    case Instruction::AM::R_D16:
    {
        auto lo = static_cast<std::uint16_t>(pBus->read(pReg->pcGet()));
        // emu_cycles(1);

        auto hi = static_cast<std::uint16_t>(pBus->read(pReg->pcGet() + 1));
        // emu_cycles(1);

        dataFetched = lo | (hi << 8);

        pReg->pcIncr();
        pReg->pcIncr();
        break;
    }
    default:
    {
        // char* msg = "Unknown addressing mode: %d";
        // logging::log(logging::Level::ERR, msg, static_cast<int>(currInst.addrMode));
        break;

    }
    }
}


bool Cpu::step()
{
    auto newOpcode = static_cast<opcode_t>(pBus->read(pReg->pcIncr()));

    // std::cout << "PC: 0x" << std::hex << (std::uint16_t)pReg->pcGet() << " OP: 0x" << std::hex << newOpcode << std::endl;
    if(Instruction::exists(newOpcode))
    {
        currOpcode = newOpcode;
        currInst   = Instruction::fetch(currOpcode);
        pReg->print();
        printf("PC: 0x%04x | OP: 0x%02x | %s\n", pReg->pcGet(), newOpcode, currInst.info);

        fetchData();
        execute();
    }
    else
    {
        // char* msg = "Unknown opcode: %d";
        // logging::log(logging::Level::ERR, msg, newOpcode);
    }

    return true;
}
}
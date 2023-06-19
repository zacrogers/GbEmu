#include "../inc/dmg01.hh"


namespace DMG01
{

address_t Cpu::execute()
{
    static auto process = Process::get(currInst.mnemonic);
    process(&currInst, &reg, &flags);
    return 0;
}


void Cpu::fetchData()
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
    {
        auto lo = static_cast<std::uint16_t>(*bus.read(reg.pcGet()));
        // emu_cycles(1);

        auto hi = static_cast<std::uint16_t>(*bus.read(reg.pcGet() + 1));
        // emu_cycles(1);

        dataFetched = lo | (hi << 8);

        reg.pcIncr();
        reg.pcIncr();
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
        // char* msg = "Unknown opcode: %d";
        // logging::log(logging::Level::ERR, msg, newOpcode);
    }

    return true;
}
}
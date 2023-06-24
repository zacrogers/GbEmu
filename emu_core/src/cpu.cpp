#include "../inc/cpu.hh"

#include <stdio.h>

namespace DMG01
{

bool Cpu::init()
{
    pReg->print();
    printf("START\n\n");
    return true;
}


address_t Cpu::execute()
{
    auto process = Process::get(currInst.mnemonic);
    if(process != nullptr)
    {
        process(&currInst, pReg, pBus, &flags);
    }
    return 0;
}

void Cpu::fetchData()
{
    pReg->memDest = 0x00;
    pReg->destIsMem = false;

    switch (currInst.addrMode)
    {
    case Instruction::AM::R_D16:
    {
        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read16(pReg->pcIncr())); pReg->pcIncr(); // increment pc twice for 16 bit

        break;
    }
    case Instruction::AM::R_R:
        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pReg->read(currInst.regB));
        break;

    case Instruction::AM::MR_R:
        pReg->setOpB(pReg->read(currInst.regB));
        pReg->memDest = pReg->read(currInst.regA);
        pReg->destIsMem = true;

        if (currInst.regA == Instruction::R::C)
        {
            pReg->memDest |= 0xFF00;
        }
        break;

    case Instruction::AM::R:
        pReg->setOpA(pReg->read(currInst.regA));
        break;

    case Instruction::AM::R_D8:
        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read(pReg->pcIncr()));
        // emu_cycles(1);
        break;

    case Instruction::AM::R_MR:
    {
        pReg->memDest = pReg->read(currInst.regB);
        pReg->destIsMem = true;

        pReg->setOpA(pReg->read(currInst.regA));

        if (currInst.regB == Instruction::R::C)
        {
            pReg->memDest |= 0xFF00;
        }
        break;
    }

    case Instruction::AM::R_HLI:
    {
        pReg->memDest = pReg->read(Register::HL);

        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read16(pReg->memDest));
        pReg->write(Register::HL, pReg->memDest + 1);

        break;
    }

    case Instruction::AM::R_HLD:
    {
        pReg->memDest = pReg->read(Register::HL);

        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read16(pReg->memDest));
        pReg->write(Register::HL, pReg->memDest - 1);

        break;
    }

    case Instruction::AM::HLI_R:
    {
        pReg->memDest = pReg->read(Register::HL);
        pReg->destIsMem = true;

        pReg->setOpB(pReg->read(currInst.regB));
        pReg->write(Register::HL, pReg->memDest + 1);

        break;
    }

    case Instruction::AM::HLD_R:
    {
        pReg->memDest = pReg->read(Register::HL);
        pReg->destIsMem = true;

        pReg->setOpB(pReg->read(currInst.regB));
        pReg->write(Register::HL, pReg->memDest - 1);

        break;
    }

    case Instruction::AM::R_A8:
    {
        address_t memAddr = pBus->read(pReg->pcIncr());

        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read(memAddr));

        // emu_cycles(1);
        break;
    }

    case Instruction::AM::A8_R:
        pReg->memDest   = pBus->read(pReg->pcIncr());
        pReg->destIsMem = true;

        pReg->setOpA(pReg->read(currInst.regB));
        pReg->setOpB(pBus->read(pReg->memDest));

        // emu_cycles(1);
        break;

    case Instruction::AM::HL_SPR:
            pReg->setOpA(pReg->read(Register::HL));
            pReg->setOpB(pBus->read(pReg->pcIncr()) + pReg->spGet());
            break;

    case Instruction::AM::D16:
            pReg->setOpA(pBus->read16(pReg->pcIncr())); pReg->pcIncr(); // increment pc twice for 16 bit
            // emu_cycles(1);
            break;

    case Instruction::AM::D8:
            pReg->setOpA(pBus->read(pReg->pcIncr()));
            // emu_cycles(1);
            break;

    case Instruction::AM::D16_R:
            pReg->setOpA(pBus->read16(pReg->pcIncr())); pReg->pcIncr(); // increment pc twice for 16 bit
            pReg->setOpB(pReg->read(currInst.regB));
            // emu_cycles(1);
            break;

    case Instruction::AM::MR_D8:
        pReg->memDest = pReg->read(currInst.regA);
        pReg->destIsMem = true;
        pReg->setOpB(pBus->read(pReg->pcIncr()));

        break;

    case Instruction::AM::MR:
        pReg->memDest = pReg->read(currInst.regA);
        pReg->destIsMem = true;

        break;

    case Instruction::AM::A16_R:
        pReg->memDest = pBus->read16(pReg->pcIncr());
        pReg->destIsMem = true;
        pReg->setOpB(pReg->read(currInst.regB));

        break;

    case Instruction::AM::R_A16:
        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(pBus->read16(pReg->pcIncr()));pReg->pcIncr();

        break;

    case Instruction::AM::IMP:
    case Instruction::AM::None:
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

    if(Instruction::exists(newOpcode))
    {
        currOpcode = newOpcode;
        if(currOpcode == 0xCB)
        {
            printf("It's a cb instruction yo\n");
        }

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
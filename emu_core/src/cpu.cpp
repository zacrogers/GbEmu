#include "../inc/cpu.hh"
#include "../inc/process.hh"

#include <stdio.h>

namespace DMG01
{

bool Cpu::init()
{
    // pReg->print();
    // printf("START\n\n");
    // printf("A:%02x F:%02x B:%02x C:%02x D:%02x E:%02x H:%02x L:%02x SP:%04x PC:%04x PCMEM:%02x,%02x,%02x,%02x\n",pReg->read(Register::A),pReg->read(Register::F),pReg->read(Register::B),pReg->read(Register::C),pReg->read(Register::D),pReg->read(Register::E),pReg->read(Register::H),pReg->read(Register::L),pReg->spGet(),pReg->pcGet(),pBus->read(pReg->pcGet()),pBus->read(pReg->pcGet()),pBus->read(pReg->pcGet()),pBus->read(pReg->pcGet()));
    pFlags = new Flags();
    return true;
}


address_t Cpu::execute()
{
    auto process = Process::get(currInst.mnemonic);
    if(process != nullptr)
    {
        process(this);
    }
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
        pReg->memDest = pReg->read(currInst.regA) << 8;
        pReg->destIsMem = true;
        // printf("VAL %04X: %02X\n\n",pReg->memDest,  pReg->getOpB());

        if (currInst.regA == Instruction::R::C)
        {
            pReg->memDest |= 0xFF00;
        }
        break;

    case Instruction::AM::R:
    {
        auto val = pReg->read(currInst.regA);
        pReg->setOpA(val);
        break;
    }

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
        auto memDest = pReg->read(Register::HL);
        auto result = pBus->read(( memDest << 8) );

        // printf("RESULT %04X: %02X\n\n", (memDest << 8), result);

        pReg->setOpA(pReg->read(currInst.regA));
        pReg->setOpB(result);

        // printf("OPA: %02X, OPB:%02X\n", pReg->getOpA(), pReg->getOpB());

        pReg->write(Register::HL, ((((memDest << 8 )) >> 8) | ((memDest << 8 )) << 8) + 1);

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
            pReg->setOpA(pBus->read(pReg->pcGet()));
            pReg->pcIncr();
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
        // pReg->print();
        // A:00 F:11 B:22 C:33 D:44 E:55 H:66 L:77 SP:8888 PC:9999 PCMEM:AA,BB,CC,DD
        // printf("PC: 0x%04x | OP: 0x%02x | %s\n", pReg->pcGet(), newOpcode, currInst.info);
        printf("A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",pReg->read(Register::A),pReg->read(Register::F),pReg->read(Register::B),pReg->read(Register::C),pReg->read(Register::D),pReg->read(Register::E),pReg->read(Register::H),pReg->read(Register::L),pReg->spGet(),pReg->pcGet()-1,pBus->read(pReg->pcGet()-1),pBus->read(pReg->pcGet()),pBus->read(pReg->pcGet()+1),pBus->read(pReg->pcGet()+2));

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
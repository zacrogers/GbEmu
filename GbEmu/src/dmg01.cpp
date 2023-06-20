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

// void fetch_data() {
//     ctx.mem_dest = 0;
//     ctx.dest_is_mem = false;

//     if (ctx.cur_inst == NULL) {
//         return;
//     }

//     switch(ctx.cur_inst->mode) {
//         case AM_IMP: return;

//         case AM_R:
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_1);
//             return;

//         case AM_R_R:
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
//             return;

//         case AM_R_D8:
//             ctx.fetched_data = bus_read(ctx.regs.pc);
//             emu_cycles(1);
//             ctx.regs.pc++;
//             return;

//         case AM_R_D16:
//         case AM_D16: {
//             u16 lo = bus_read(ctx.regs.pc);
//             emu_cycles(1);

//             u16 hi = bus_read(ctx.regs.pc + 1);
//             emu_cycles(1);

//             ctx.fetched_data = lo | (hi << 8);

//             ctx.regs.pc += 2;

//             return;
//         }

//         case AM_MR_R:
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
//             ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
//             ctx.dest_is_mem = true;

//             if (ctx.cur_inst->reg_1 == RT_C) {
//                 ctx.mem_dest |= 0xFF00;
//             }

//             return;

//         case AM_R_MR: {
//             u16 addr = cpu_read_reg(ctx.cur_inst->reg_2);

//             if (ctx.cur_inst->reg_2 == RT_C) {
//                 addr |= 0xFF00;
//             }

//             ctx.fetched_data = bus_read(addr);
//             emu_cycles(1);

//         } return;

//         case AM_R_HLI:
//             ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2));
//             emu_cycles(1);
//             cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
//             return;

//         case AM_R_HLD:
//             ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2));
//             emu_cycles(1);
//             cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
//             return;

//         case AM_HLI_R:
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
//             ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
//             ctx.dest_is_mem = true;
//             cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
//             return;

//         case AM_HLD_R:
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
//             ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
//             ctx.dest_is_mem = true;
//             cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
//             return;

//         case AM_R_A8:
//             ctx.fetched_data = bus_read(ctx.regs.pc);
//             emu_cycles(1);
//             ctx.regs.pc++;
//             return;

//         case AM_A8_R:
//             ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
//             ctx.dest_is_mem = true;
//             emu_cycles(1);
//             ctx.regs.pc++;
//             return;

//         case AM_HL_SPR:
//             ctx.fetched_data = bus_read(ctx.regs.pc);
//             emu_cycles(1);
//             ctx.regs.pc++;
//             return;

//         case AM_D8:
//             ctx.fetched_data = bus_read(ctx.regs.pc);
//             emu_cycles(1);
//             ctx.regs.pc++;
//             return;

//         case AM_A16_R:
//         case AM_D16_R: {
//             u16 lo = bus_read(ctx.regs.pc);
//             emu_cycles(1);

//             u16 hi = bus_read(ctx.regs.pc + 1);
//             emu_cycles(1);

//             ctx.mem_dest = lo | (hi << 8);
//             ctx.dest_is_mem = true;

//             ctx.regs.pc += 2;
//             ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);

//         } return;

//         case AM_MR_D8:
//             ctx.fetched_data = bus_read(ctx.regs.pc);
//             emu_cycles(1);
//             ctx.regs.pc++;
//             ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
//             ctx.dest_is_mem = true;
//             return;

//         case AM_MR:
//             ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
//             ctx.dest_is_mem = true;
//             ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_1));
//             emu_cycles(1);
//             return;

//         case AM_R_A16: {
//             u16 lo = bus_read(ctx.regs.pc);
//             emu_cycles(1);

//             u16 hi = bus_read(ctx.regs.pc + 1);
//             emu_cycles(1);

//             u16 addr = lo | (hi << 8);

//             ctx.regs.pc += 2;
//             ctx.fetched_data = bus_read(addr);
//             emu_cycles(1);

//             return;
//         }

//         default:
//             printf("Unknown Addressing Mode! %d (%02X)\n", ctx.cur_inst->mode, ctx.cur_opcode);
//             exit(-7);
//             return;
//     }
// }
void Cpu::fetchData()
{
    pReg->memDest = 0x00;
    pReg->destIsMem = false;

    switch (currInst.addrMode)
    {
    case Instruction::AM::IMP:
    case Instruction::AM::R_D16:
    {
        auto lo = static_cast<std::uint16_t>(pBus->read(pReg->pcIncr()));
        // emu_cycles(1);

        auto hi = static_cast<std::uint16_t>(pBus->read(pReg->pcIncr()));
        // emu_cycles(1);

        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->setOpB(lo | (hi << 8));

        break;
    }
    case Instruction::AM::R_R:
        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->setOpB(pReg->get8(currInst.regB));
        break;

    case Instruction::AM::MR_R:
        pReg->setOpB(pReg->get8(currInst.regB));
        pReg->memDest = pReg->get8(currInst.regA);
        pReg->destIsMem = true;

        if (currInst.regA == Instruction::R::C)
        {
            pReg->memDest |= 0xFF00;
        }
        break;

    case Instruction::AM::R:
        pReg->setOpA(pReg->get8(currInst.regA));
        break;

    case Instruction::AM::R_D8:
        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->setOpB(pBus->read(pReg->pcIncr()));
        // emu_cycles(1);
        break;

    case Instruction::AM::R_MR:
    {
        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->memDest = pReg->get8(currInst.regB);
        pReg->destIsMem = true;

        if (currInst.regB == Instruction::R::C)
        {
            pReg->memDest |= 0xFF00;
        }
        break;
    }

    case Instruction::AM::R_HLI:
    {
        auto regBVal = pReg->get16(currInst.regB);

        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->setOpB(*pBus->read16(regBVal));

        pReg->set16(currInst.regB, regBVal++);

        break;
    }

    case Instruction::AM::R_HLD:
    {
        auto memAddr = pReg->get16(currInst.regB);

        pReg->setOpA(pReg->get8(currInst.regA));
        pReg->setOpB(*pBus->read16(memAddr));

        pReg->set16(currInst.regB, memAddr--);

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
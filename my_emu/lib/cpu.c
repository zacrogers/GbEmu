#include <cpu.h>
#include <bus.h>

cpu_context ctx = {0};

void cpu_init() 
{
    ctx.regs.pc = 0x100;
    ctx.regs.a = 0x01;
}

static void fetch_instruction()
{
    ctx.curr_opcode = bus_read(ctx.regs.pc++);
    ctx.curr_inst = instruction_by_opcode(ctx.curr_opcode);

    if(ctx.curr_inst == NULL)
    {
        printf("Unknown instruction %02X\n", ctx.curr_opcode);
        exit(-7);
    }
}

static void fetch_data()
{
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch (ctx.curr_inst->mode)
    {
        case AM_IMP:
            return;

        case AM_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_1);
            return;

        case AM_R_D8:
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_R_D16:
        {
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc +1);
            emu_cycles(1);

            ctx.fetched_data = lo | (hi << 8);

            ctx.regs.pc += 2;
             
            return;
        }
        
        default:
            printf("Unknown addressing mode: %d\n", ctx.curr_inst->mode);
            exit(-7);
            return;
    }
}

static void execute()
{
    printf("Not executing yet...\n");
}

bool cpu_step() 
{
    if(!ctx.halted)
    {
        fetch_instruction();
        fetch_data();
        printf("Exectuting instruction %02X    PC: %04X\n", ctx.curr_opcode, ctx.regs.pc);
        execute();
    }

    return true;
}

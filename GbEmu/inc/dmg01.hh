#pragma once

#include <cstdint>
#include <map>
#include <functional>
#include <string>

#include "common.hh"
#include "logging.hh"
#include "instructions.hh"
#include "bus.hh"
#include "../inc/registers.hh"
#include "../inc/process.hh"
#include "../inc/cart.hh"

namespace DMG01
{

class Cpu
{
public:
    Cpu() {}
    Cpu(Registers* reg, Bus* bus): pReg(reg), pBus(bus){}

    bool      init();
    address_t execute();
    void      fetchData();
    bool      step();

private:
    Registers         *pReg       { };
    Bus               *pBus       { };

    Flags             flags       { };
    Instruction::ctx  currInst    { };
    opcode_t          currOpcode  { 0 };
    std::uint16_t     dataFetched { 0 };

    bool              halted      { false };
    bool              stepping    { false };

};

}
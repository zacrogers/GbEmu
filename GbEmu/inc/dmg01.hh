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

namespace DMG01
{

class Cpu
{
public:
    Cpu() {}

    address_t execute();
    void      fetchData();
    bool      step();

private:
    Registers         reg         { };
    Bus               bus         { };
    Flags             flags       { };
    Instruction::ctx  currInst    { };
    opcode_t          currOpcode  { 0 };
    std::uint16_t     dataFetched { 0 };
    address_t         memDest     { 0 };
    bool              destIsMem   { false };
    bool              halted      { false };
    bool              stepping    { false };
};

}
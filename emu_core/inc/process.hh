#pragma once

#include <functional>

#include "common.hh"
#include "instructions.hh"
#include "cpu.hh"


namespace DMG01
{
namespace Process
{

using callback = std::function<void(Cpu *)>;
callback get(Instruction::MN mnemonic);

};
}
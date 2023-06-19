#pragma once

#include <map>
#include <functional>

#include "../inc/common.hh"
#include "../inc/logging.hh"
#include "../inc/instructions.hh"
#include "../inc/bus.hh"
#include "../inc/registers.hh"

namespace DMG01
{
namespace Process
{

typedef std::function<void(const Instruction::ctx*, Registers*, Flags*)> callback;
callback get(Instruction::MN mnemonic);

};
}
#include "../inc/instructions.hh"
namespace DMG01
{

namespace Instruction
{
const ctx fetch(opcode_t op)  { return inst[op]; }
bool      exists(opcode_t op) { return &inst[op] != nullptr; }
}
}
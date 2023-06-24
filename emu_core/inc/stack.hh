#pragma once

#include <cstdint>

namespace DMG01
{
namespace Stack
{
void push(std::uint8_t);
void push(std::uint16_t);
void pop(std::uint8_t);
void pop(std::uint16_t);
}
}
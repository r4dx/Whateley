#pragma once

#include "vm/memory/memory.h"
#include "vm/lang/command.h"

#include <unordered_map>

namespace eeagl::vm::exec {
    struct Context {
        memory::Memory& memory;

        memory::MemoryAddress ip;
        std::unordered_map<lang::Register, lang::CellCommandPointer> registers;
        std::unordered_map<lang::DirectionRegister, lang::Direction> directionRegisters;

        Context(memory::Memory& memory, memory::MemoryAddress ip);
    };
}
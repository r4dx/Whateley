#include "execution.h"
#include "vm/lang/command.h"

#include <unordered_map>

namespace eeagl::vm::exec {
    Context::Context(memory::Memory& memory, memory::MemoryAddress ip) : memory(memory), ip(ip) {
        for (auto reg : lang::REGISTERS)
            registers[reg] = lang::toPointer(0);

        for (auto dirReg : lang::DIRECTION_REGISTERS)
            directionRegisters[dirReg] = lang::Direction::Same;
    }
}
#pragma once

#include <initializer_list>
#include <tuple>

#include "command.h"
#include "vm/memory/memory_address.h"
#include "vm/memory/memory_dump.h"

namespace eeagl::vm::lang::command::block {
    typedef std::initializer_list<std::tuple<lang::command::RawCommand, int>> Block;

    int getCommandsCount(Block commandCounts);
    void fillWithCommands(memory::MemoryDump& dump, memory::MemoryAddress addr, Block commandCounts);
    bool hasCommands(memory::MemoryDump& dump, memory::MemoryAddress addr, Block commandCounts);
}
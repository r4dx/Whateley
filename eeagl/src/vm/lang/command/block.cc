#include "block.h"

namespace eeagl::vm::lang::command::block {
    int getCommandsCount(Block commandCounts) {
        int count = 0;
        for (auto commandCount : commandCounts)
            count += std::get<1>(commandCount);
        return count;
    }

    void fillWithCommands(memory::MemoryDump& dump, memory::MemoryAddress addr, Block commandCounts) {
        assert(getCommandsCount(commandCounts) <= lang::CELL_SIZE - addr.index);
        std::vector<lang::command::RawCommand> commands;

        for (auto commandCount : commandCounts)
            commands.insert(commands.end(), std::get<1>(commandCount), std::get<0>(commandCount));

        int commandsIndex = addr.index;
        for (auto command : commands)
            dump.cells[addr.y][addr.x].commands[commandsIndex++] = command;
    }

    bool hasCommands(memory::MemoryDump& dump, memory::MemoryAddress addr, Block commandCounts) {
        if (getCommandsCount(commandCounts) > lang::CELL_SIZE - addr.index)
            return false;

        int index = addr.index;
        for (auto commandCount : commandCounts) {
            for (int i = 0; i < std::get<1>(commandCount); i++) {
                if (dump.cells[addr.y][addr.x].commands[index] != std::get<0>(commandCount))
                    return false;
                index++;
            }
        }
        return true;
    }
}
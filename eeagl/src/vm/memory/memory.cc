#include "memory.h"
#include "memory_dump.h"
#include <cstring>

namespace eeagl::vm::memory {
	MemoryAddress Memory::toAddress(int x, int y, lang::CellCommandPointer index) const {
		return dump->toAddress(x, y, index);
	}

    DereferenceResult Memory::dereference(MemoryAddress address) const {
        if (!dump->isCompatible(address))
            return { false, Error::INVALID_ADDRESS };
        
        auto command = dump->cells[address.y][address.x].commands[(int)address.index];
        return { true, Error::NONE, command };
    }

    SwapResult Memory::swap(MemoryAddress address_1, MemoryAddress address_2) {
        if (!dump->isCompatible(address_1) || !dump->isCompatible(address_2))
            return { false, Error::INVALID_ADDRESS };

        auto tmp = dump->cells[address_1.y][address_1.x].commands[address_1.index];
        dump->cells[address_1.y][address_1.x].commands[address_1.index] = 
            dump->cells[address_2.y][address_2.x].commands[address_2.index];
        dump->cells[address_2.y][address_2.x].commands[address_2.index] = tmp;
        return { true };
    }

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
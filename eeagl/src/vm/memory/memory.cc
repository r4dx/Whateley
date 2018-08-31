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

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
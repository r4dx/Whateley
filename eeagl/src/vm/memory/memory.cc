#include "memory.h"
#include "memory_dump.h"
#include <cstring>

namespace eeagl::vm::memory {
	MemoryAddress Memory::toAddress(int x, int y, lang::CellCommandPointer index) const {
		return dump->toAddress(x, y, index);
	}

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
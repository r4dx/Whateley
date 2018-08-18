#include "memory.h"
#include "memory_dump.h"
#include <cstring>

namespace eeagl::vm::memory {
    bool MemoryAddress::operator==(const MemoryAddress& rhs) const {
        return x == rhs.x && y == rhs.y && index == rhs.index;
    }
    bool MemoryAddress::operator!=(const MemoryAddress& rhs) const {
        return !operator==(rhs);
    }

	MemoryAddress& MemoryAddress::operator++() {
		if ((int)index + 1 < dimZ)
			index = lang::toPointer((int)index + 1);
		else {
			index = lang::toPointer(0);

			if (++x >= dimX) {
				x = 0;
				if (++y >= dimY)
					y = 0;
			}
		}
		return *this;
	}

    int MemoryAddress::toFlatIndex() const {
        return y * dimZ * dimX + x * dimZ + (int)index;
    }

    MemoryAddress MemoryAddress::fromFlatIndex(int index, int dimX, int dimY, int dimZ) {
        int y = index / (dimZ * dimX);
        int x = (index - y * dimZ * dimX) / dimZ;
        int z = index - y * dimZ * dimX - x * dimZ;

        return MemoryAddress(x, y, std::byte(z), dimX, dimY, dimZ);
    }

    SetMemoryResult Memory::set(MemoryAddress, lang::RawCommand command) {
        SetMemoryResult result;
        result.error = SetMemoryResult::INVALID_ADDRESS;
        result.succeed = false;
        return result;
    }

	GetCellResult Memory::get(const CellAddress& address) const {
        GetCellResult result;
        result.succeed = false;
        result.error = GetCellResult::Error::INVALID_ADDRESS;
        return result;
    }

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
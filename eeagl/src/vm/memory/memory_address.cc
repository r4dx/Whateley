#include "memory_address.h"

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
}
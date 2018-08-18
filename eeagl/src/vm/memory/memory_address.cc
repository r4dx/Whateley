#include "memory_address.h"
#include <cassert>

namespace eeagl::vm::memory {
	CellAddress::CellAddress(int x, int y) : x(x), y(y) {
		assert(x >= 0 && y >= 0);
	}

	MemoryAddress::MemoryAddress(int x, int y, lang::CellCommandPointer index, int dimX, int dimY, int dimZ) :
		CellAddress(x, y),
		index(index),
		dimX(dimX),
		dimY(dimY),
		dimZ(dimZ) {
	
		assert(dimX > 0 && dimY > 0 && dimZ > 0);
	}

	MemoryAddress::MemoryAddress(std::tuple<int, int, lang::CellCommandPointer> addr, std::tuple<int, int, int> dimensions) :
		MemoryAddress(std::get<0>(addr), std::get<1>(addr), std::get<2>(addr), 
		std::get<0>(dimensions),
		std::get<1>(dimensions),
		std::get<2>(dimensions)) { }

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
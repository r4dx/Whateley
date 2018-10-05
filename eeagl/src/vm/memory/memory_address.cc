#include "memory_address.h"
#include <cassert>

namespace eeagl::vm::memory {
	MemoryAddress::MemoryAddress(int x, int y, lang::CellCommandPointer index, int dimX, int dimY, int dimZ) :
		x(x),
        y(y),
		index(index),
		dimX(dimX),
		dimY(dimY),
		dimZ(dimZ) {

		assert(x >= 0 && y >= 0 && dimX > 0 && dimY > 0 && dimZ > 0);
	}

	bool MemoryAddress::operator==(const MemoryAddress& rhs) const {
		return x == rhs.x && y == rhs.y && index == rhs.index;
	}
	bool MemoryAddress::operator!=(const MemoryAddress& rhs) const {
		return !operator==(rhs);
	}

	MemoryAddress& MemoryAddress::operator++() {
		if (index + 1 < dimZ)
			index = index + 1;
		else {
			index = 0;

			if (++x >= dimX) {
				x = 0;
				if (++y >= dimY)
					y = 0;
			}
		}
		return *this;
	}

    int rotator(int value, int max) {
        if (value < 0)
            return max - 1;
        if (value >= max)
            return 0;
        return value;
    }

    std::optional<MemoryAddress> MemoryAddress::neighborCell(const lang::Direction direction) const {
        switch (direction)
        {
        case lang::Direction::Up:
            return MemoryAddress(x, rotator(y - 1, dimY), 0, dimX, dimY, dimZ);
        case lang::Direction::Right:
            return MemoryAddress(rotator(x + 1, dimX), y, 0, dimX, dimY, dimZ);
        case lang::Direction::Left:
            return MemoryAddress(rotator(x - 1, dimX), y, 0, dimX, dimY, dimZ);
        case lang::Direction::Down:
            return MemoryAddress(x, rotator(y + 1, dimY), 0, dimX, dimY, dimZ);
        case lang::Direction::Same:
            return MemoryAddress(x, y, 0, dimX, dimY, dimZ);
        }
        return std::nullopt;
    }

    int MemoryAddress::toFlatIndex() const {
		return y * dimZ * dimX + x * dimZ + (int)index;
	}

	MemoryAddress MemoryAddress::fromFlatIndex(int index, int dimX, int dimY, int dimZ) {
		int y = index / (dimZ * dimX);
		int x = (index - y * dimZ * dimX) / dimZ;
		int z = index - y * dimZ * dimX - x * dimZ;

		return MemoryAddress(x, y, z, dimX, dimY, dimZ);
	}
}
#pragma once

#include "vm/lang/command.h"

#include <tuple>

namespace eeagl::vm::memory {

    class MemoryAddress {
	public:
        int x;
        int y;
        lang::CellCommandPointer index;

		MemoryAddress(int x, int y, lang::CellCommandPointer index, int dimX, int dimY, int dimZ);
		MemoryAddress(std::tuple<int, int, lang::CellCommandPointer> addr, std::tuple<int, int, int> dimensions);

		bool operator==(const MemoryAddress& rhs) const;
		bool operator!=(const MemoryAddress& rhs) const;
		MemoryAddress& operator++();

        MemoryAddress neighborCell(const lang::Direction direction) const;
		int toFlatIndex() const;
		static MemoryAddress fromFlatIndex(int index, int dimX, int dimY, int dimZ);
	private:

		int dimX;
		int dimY;
		int dimZ;
	};
}
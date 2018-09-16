#pragma once

#include "vm/lang/command.h"
#include <optional>
#include <tuple>

namespace eeagl::vm::memory {

    class MemoryAddress {
	public:
        int x;
        int y;
        lang::CellCommandPointer index;

		MemoryAddress(int x, int y, lang::CellCommandPointer index, int dimX, int dimY, int dimZ);

		bool operator==(const MemoryAddress& rhs) const;
		bool operator!=(const MemoryAddress& rhs) const;
		MemoryAddress& operator++();

        std::optional<MemoryAddress> neighborCell(const lang::Direction direction) const;
		int toFlatIndex() const;
		static MemoryAddress fromFlatIndex(int index, int dimX, int dimY, int dimZ);
	private:

		int dimX;
		int dimY;
		int dimZ;
	};
}
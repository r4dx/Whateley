#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <istream>
#include <ostream>
#include <tuple>
#include "vm/lang/command.h"
#include "memory_dump.h"

namespace eeagl::vm::memory {
    struct CellAddress {
        int x;
        int y;

        CellAddress(int x, int y) : x(x), y(y) {}
    };

    struct MemoryAddress : CellAddress {
        lang::CellCommandPointer index;
        MemoryAddress(int x, int y, lang::CellCommandPointer index, int dimX, int dimY, int dimZ) : 
			CellAddress(x, y), 
			index(index),
			dimX(dimX),
			dimY(dimY),
			dimZ(dimZ) {}
        MemoryAddress(std::tuple<int, int, lang::CellCommandPointer> addr, std::tuple<int, int, int> dimensions) : 
			CellAddress(std::get<0>(addr), std::get<1>(addr)), index(std::get<2>(addr)),
				dimX(std::get<0>(dimensions)),
				dimY(std::get<1>(dimensions)),
				dimZ(std::get<2>(dimensions)) {}

        bool operator==(const MemoryAddress& rhs) const;
        bool operator!=(const MemoryAddress& rhs) const;
		MemoryAddress& operator++();

        int toFlatIndex() const;
        static MemoryAddress fromFlatIndex(int index, int dimX, int dimY, int dimZ);
	private:
		int dimX;
		int dimY;
		int dimZ;
    };

    struct GetCellResult {
        bool succeed;
        enum Error { NONE, INVALID_ADDRESS } error;
        std::shared_ptr<const lang::Cell> cell;
    };

    struct SetMemoryResult {
        bool succeed;
        enum Error { NONE, INVALID_ADDRESS } error;
    };

    class Memory {
    public:
        GetCellResult get(const CellAddress& address) const;
        Memory(const std::shared_ptr<MemoryDump> dump);
    protected:
        SetMemoryResult set(MemoryAddress, lang::RawCommand command);
    private:
        std::shared_ptr<MemoryDump> dump;
    };
}
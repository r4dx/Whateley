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
        MemoryAddress(int x, int y, lang::CellCommandPointer index) : CellAddress(x, y), index(index) {}
        MemoryAddress(std::tuple<int, int, lang::CellCommandPointer> addr) : CellAddress(std::get<0>(addr), std::get<1>(addr)), index(std::get<2>(addr)) {}

        bool operator==(const MemoryAddress& rhs) const;
        bool operator!=(const MemoryAddress& rhs) const;

        int toFlatIndex(int dimX, int dimZ) const;
        static MemoryAddress fromFlatIndex(int index, int dimX, int dimZ);
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
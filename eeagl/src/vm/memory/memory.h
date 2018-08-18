#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <istream>
#include <ostream>
#include <tuple>
#include "vm/lang/command.h"
#include "memory_dump.h"
#include "memory_address.h"

namespace eeagl::vm::memory {
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
		MemoryAddress toAddress(int x, int y, lang::CellCommandPointer index) const;
        Memory(const std::shared_ptr<MemoryDump> dump);
    protected:
        SetMemoryResult set(MemoryAddress, lang::RawCommand command);
    private:
        std::shared_ptr<MemoryDump> dump;
    };
}
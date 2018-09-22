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

    enum Error { NONE, INVALID_ADDRESS };

    struct GetCellResult {
        bool succeed;
        Error error;
        std::shared_ptr<const lang::Cell> cell;
    };

    struct SetMemoryResult {
        bool succeed;
        Error error;
    };

    struct DereferenceResult {
        bool succeed;
        Error error;
        lang::RawCommand value;
    };

    struct SwapResult {
        bool succeed;
        Error error;
    };

    class Memory {
    public:
        MemoryAddress toAddress(int x, int y, lang::CellCommandPointer index) const;
        DereferenceResult dereference(MemoryAddress address) const;
        SwapResult swap(MemoryAddress address_1, MemoryAddress address_2);
        Memory(const std::shared_ptr<MemoryDump> dump);
    private:
        std::shared_ptr<MemoryDump> dump;
    };
}
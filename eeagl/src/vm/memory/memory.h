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

    class Memory {
    public:
        MemoryAddress toAddress(int x, int y, lang::CellCommandPointer index) const;
        //DereferenceResult dereference(MemoryAddress address);
        Memory(const std::shared_ptr<MemoryDump> dump);
    private:
        std::shared_ptr<MemoryDump> dump;
    };
}
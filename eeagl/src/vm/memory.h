#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <istream>
#include <ostream>
#include "command.h"
#include "memory_dump.h"

namespace eeagle {
    namespace vm {
        namespace memory {
            struct CellAddress {
                int x;
                int y;

                CellAddress(int x, int y) : x(x), y(y) {}
            };

            struct MemoryAddress : CellAddress {
                std::byte index;
                MemoryAddress(int x, int y, std::byte index) : CellAddress(x, y), index(index) {}
            };

            struct GetCellResult {
                bool succeed;
                enum Error { NONE, INVALID_ADDRESS } error;
                std::shared_ptr<const command::Cell> cell;
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
                SetMemoryResult set(MemoryAddress, command::RawCommand command);
            private:
                std::shared_ptr<MemoryDump> dump;
            };
        }
    }
}
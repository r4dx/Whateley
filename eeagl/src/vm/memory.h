#pragma once

#include <cstddef>
#include <memory>

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

            struct Cell {
            };

            struct GetCellResult {
                bool isSuccess;
                enum Error { NONE, INVALID_ADDRESS } error;
                std::shared_ptr<const Cell> cell;
            };

            class Memory {
            public:
                GetCellResult get(const CellAddress& address) const;

            };

        }
    }
}
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <istream>
#include <ostream>
#include "command.h"

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
                bool succeed;
                enum Error { NONE, INVALID_ADDRESS } error;
                std::shared_ptr<const Cell> cell;
            };

            struct SetMemoryResult {
                bool succeed;
                enum Error { NONE, INVALID_ADDRESS } error;
            };

            class Memory {
            protected:
                SetMemoryResult set(MemoryAddress, command::RawCommand command);
            public:
                GetCellResult get(const CellAddress& address) const;
            };

            struct CreateDumpableMemoryResult;

            class StreamDumpableMemory : public Memory {
            private:
                StreamDumpableMemory();
            public:
                static CreateDumpableMemoryResult create(std::istream& is);
                void save(std::ostream& os);
            };

            struct CreateDumpableMemoryResult {
                bool succeed;
                enum Error {
                    NONE,
                    INVALID_SIGNATURE,
                    READ_ERROR
                } error;
                std::shared_ptr<StreamDumpableMemory> memory;
            };
        }
    }
}
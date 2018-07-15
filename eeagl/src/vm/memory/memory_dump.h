#pragma once
#include <string>
#include <istream>
#include <ostream>
#include <vector>
#include <memory>
#include "vm/lang/command.h"

namespace eeagl {
    namespace vm {
        namespace memory {
            struct MemoryDumpHeader {
                std::string signature;
                int version;
                int xDimension;
                int yDimension;
            };

            struct MemoryDump;

            struct ReadDumpResult {
                bool isSuccess;
                enum Error {
                    NONE,
                    READ_ERROR,
                    INVALID_SIGNATURE,
                    INVALID_VERSION,
                    INCORRECT_DIMENSIONS
                } error;
                std::shared_ptr<MemoryDump> result;
            };

            struct WriteDumpResult {
                bool isSuccess;
                enum Error { NONE, WRITE_ERROR } error;
            };

            struct MemoryDump {
                MemoryDumpHeader header;
                std::vector< std::vector < lang::Cell > > cells;

                static ReadDumpResult read(std::istream& is);
                WriteDumpResult write(const MemoryDump& dump, std::ostream& os);

                static const std::string SIGNATURE;
                static const int CURRENT_VERSION = 0 + lang::CELL_SIZE;
                static const int MAX_DIMENSION = 256;
            };
        }
    }
}
#pragma once
#include <string>
#include <istream>
#include <ostream>
#include <vector>
#include <memory>
#include "../command.h"
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>

namespace eeagl {
    namespace vm {
        namespace memory {
            struct MemoryDumpHeader {
                std::string signature;
                int version;
                int xDimension;
                int yDimension;

                template<class Archive>
                void serialize(Archive& archive) {
                    archive(signature, version, xDimension, yDimension);
                }
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
                command::Cell* cells;
                ~MemoryDump();

                static ReadDumpResult read(std::istream& is);
                WriteDumpResult write(const MemoryDump& dump, std::ostream& os);

                static const std::string SIGNATURE;
                static const int CURRENT_VERSION = 0 + command::CELL_SIZE;
                static const int MAX_DIMENSION = 256;
            };
        }
    }
}
#pragma once

#include <string>
#include <istream>
#include <ostream>
#include <vector>
#include <memory>
#include "vm/lang/command.h"
#include "memory_address.h"

namespace eeagl::vm::memory {
    struct MemoryDumpHeader {
        std::string signature;
        int version;

        static const std::string SIGNATURE;
        static const int CURRENT_VERSION = 0 + lang::CELL_SIZE;
    };

    struct MemoryDump;

    struct ReadDumpResult {
        bool succeed;
        enum Error {
            NONE,
            READ_ERROR,
            INVALID_SIGNATURE,
            INVALID_VERSION
        } error;
        std::shared_ptr<MemoryDump> result;
    };

    struct WriteDumpResult {
        bool succeed;
        enum Error { NONE, WRITE_ERROR } error;
    };

    struct MemoryDump {
        MemoryDumpHeader header;
        std::vector< std::vector < lang::Cell > > cells;
		MemoryAddress toAddress(int x, int y, lang::CellCommandPointer index) const;

        static ReadDumpResult read(std::istream& is);
        WriteDumpResult write(std::ostream& os);
    };
}
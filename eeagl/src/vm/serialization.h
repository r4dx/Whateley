#pragma once
#include "command.h"
#include "memory/memory_dump.h"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace eeagl {
    namespace vm {
        template <class Archive>
        void serialize(Archive& archive, command::Cell& cell) {
            archive(cell.commands);
        }

        template<class Archive>
        void serialize(Archive& archive, memory::MemoryDumpHeader& header) {
            archive(header.signature, header.version, header.xDimension, header.yDimension);
        }

        template<class Archive>
        void serialize(Archive& archive, memory::MemoryDump& dump) {
            archive(dump.header, dump.cells);
        }
    }
}
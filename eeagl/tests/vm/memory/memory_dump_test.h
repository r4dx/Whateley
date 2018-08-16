#pragma once
#include "gtest/gtest.h"
#include "vm/memory/memory_dump.h"
#include <sstream>
#include <string>

namespace eeagl::vm::memory {
    class MemoryDumpTest : public ::testing::Test {
    public:
        static std::shared_ptr<MemoryDump> createSimpleMemoryDump(int x = 1, int y = 1);
    protected:
        std::shared_ptr<std::istream> createUnderlyingStream(
            const std::string signature = MemoryDumpHeader::SIGNATURE,
            int version = MemoryDumpHeader::CURRENT_VERSION,
            int xDimension = 1,
            int yDimension = 1);
    };
}
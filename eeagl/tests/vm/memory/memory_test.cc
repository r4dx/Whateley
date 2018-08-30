#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <cstddef>

#include <cereal/archives/portable_binary.hpp>

#include "vm/memory/memory.h"
#include "vm/memory/memory_dump.h"
#include "vm/lang/command.h"
#include "memory_dump_test.h"

namespace eeagl::vm::memory {
    class MemoryTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            auto dump = MemoryDumpTest::createSimpleMemoryDump();
            memory = std::make_shared<Memory>(dump);
        }
        std::shared_ptr<Memory> memory;
    };
}
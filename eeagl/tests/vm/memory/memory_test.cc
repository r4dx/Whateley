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
            dump = MemoryDumpTest::createSimpleMemoryDump();
            memory = std::make_shared<Memory>(dump);
        }
        std::shared_ptr<MemoryDump> dump;
        std::shared_ptr<Memory> memory;
    };

    TEST_F(MemoryTest, DereferenceCommonCase) {
        lang::RawCommand command;
        command.op = lang::Operator::Stop;
        auto address = memory->toAddress(0, 0, 0);
        dump->cells[address.y][address.x].commands[address.index] = command;
        auto result = memory->dereference(address);
        EXPECT_TRUE(result.succeed);
        EXPECT_EQ(result.value, command);
    }

    TEST_F(MemoryTest, DereferenceInvalidAddress) {
        auto address = MemoryAddress(99, 0, 0, 100, 100, 100);
        auto result = memory->dereference(address);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, Error::INVALID_ADDRESS);
    }
}
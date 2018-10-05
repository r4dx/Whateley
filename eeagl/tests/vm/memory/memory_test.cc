#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <cstddef>

#include <cereal/archives/portable_binary.hpp>

#include "vm/memory/memory.h"
#include "vm/memory/memory_dump.h"
#include "vm/lang/command/command.h"
#include "vm/lang/command/factory.h"
#include "memory_dump_test.h"

namespace eeagl::vm::memory {
    class MemoryTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            dump = MemoryDumpTest::createSimpleMemoryDump(5, 5);
            memory = std::make_shared<Memory>(dump);
        }

        SwapResult doTestSwap(const MemoryAddress& address_1, const MemoryAddress& address_2, 
            const lang::command::RawCommand& command_1, const lang::command::RawCommand& command_2) {

            dump->cells[address_1.y][address_1.x].commands[address_1.index] = command_1;
            dump->cells[address_2.y][address_2.x].commands[address_2.index] = command_2;
            return memory->swap(address_1, address_2);
        }
        std::shared_ptr<MemoryDump> dump;
        std::shared_ptr<Memory> memory;
    };

    TEST_F(MemoryTest, DereferenceCommonCase) {
        lang::command::RawCommand command;
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

    TEST_F(MemoryTest, SwapInvalidAddress1) {
        auto invalid_address = MemoryAddress(99, 0, 0, 100, 100, 100);
        auto address = memory->toAddress(0, 0, 0);
        auto result = memory->swap(address, invalid_address);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, Error::INVALID_ADDRESS);
    }

    TEST_F(MemoryTest, SwapInvalidAddress2) {
        auto invalid_address = MemoryAddress(99, 0, 0, 100, 100, 100);
        auto address = memory->toAddress(0, 0, 0);
        auto result = memory->swap(invalid_address, address);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, Error::INVALID_ADDRESS);
    }

    TEST_F(MemoryTest, SwapCommonCase) {
        auto command_1 = lang::command::factory::build(lang::Operator::Stop);
        auto command_2 = lang::command::factory::build(lang::Operator::Increment, lang::Register::Register_1);

        auto address_1 = memory->toAddress(0, 0, 0);
        auto address_2 = memory->toAddress(1, 2, 3);
        auto result = doTestSwap(address_1, address_2, command_1, command_2);
        EXPECT_TRUE(result.succeed);
        EXPECT_EQ(dump->cells[address_1.y][address_1.x].commands[address_1.index], command_2);
        EXPECT_EQ(dump->cells[address_2.y][address_2.x].commands[address_2.index], command_1);
    }
    
    TEST_F(MemoryTest, SwapBlocksTilEndCommonCaseFirstAddressBigger) {
        EXPECT_TRUE(false);
    }

    TEST_F(MemoryTest, SwapBlocksTilEndCommonCaseSecondAddressBigger) {
        EXPECT_TRUE(false);
    }

    TEST_F(MemoryTest, SwapBlocksTilEndSameAddresses) {
        EXPECT_TRUE(false);
    }


    TEST_F(MemoryTest, SwapBlocksTilInvalidAddress1) {
        EXPECT_TRUE(false);
    }

    TEST_F(MemoryTest, SwapBlocksTilInvalidAddress2) {
        EXPECT_TRUE(false);
    }
}
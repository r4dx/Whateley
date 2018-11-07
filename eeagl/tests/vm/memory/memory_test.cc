#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <cstddef>
#include <tuple>
#include <initializer_list>

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

        int getCommandsCount(std::initializer_list<std::tuple<lang::command::RawCommand, int>> commandCounts) {
            int count = 0;
            for (auto commandCount : commandCounts)
                count += std::get<1>(commandCount);
            return count;
        }

        void fillWithCommands(MemoryAddress addr, 
            std::initializer_list<std::tuple<lang::command::RawCommand, int>> commandCounts) {

            assert(getCommandsCount(commandCounts) <= lang::CELL_SIZE - addr.index);
            std::vector<lang::command::RawCommand> commands;

            for (auto commandCount : commandCounts)
                commands.insert(commands.end(), std::get<1>(commandCount), std::get<0>(commandCount));

            int commandsIndex = addr.index;
            for (auto command : commands)
                dump->cells[addr.y][addr.x].commands[commandsIndex++] = command;
                
        }

        bool hasCommands(MemoryAddress addr,
            std::initializer_list<std::tuple<lang::command::RawCommand, int>> commandCounts) {

            if (getCommandsCount(commandCounts) > lang::CELL_SIZE - addr.index)
                return false;

            int index = addr.index;
            for (auto commandCount : commandCounts) {
                for (int i = 0; i < std::get<1>(commandCount); i++) {
                    if (dump->cells[addr.y][addr.x].commands[index] != std::get<0>(commandCount))
                        return false;
                    index++;
                }
            }
            return true;
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
        using namespace lang::command;

        auto command_1 = factory::build(lang::Operator::Stop);
        auto command_2 = factory::build(lang::Operator::Increment, lang::Register::Register_1);

        auto address_1 = memory->toAddress(0, 0, 0);
        auto address_2 = memory->toAddress(1, 2, 3);
        auto result = doTestSwap(address_1, address_2, command_1, command_2);
        EXPECT_TRUE(result.succeed);
        EXPECT_EQ(dump->cells[address_1.y][address_1.x].commands[address_1.index], command_2);
        EXPECT_EQ(dump->cells[address_2.y][address_2.x].commands[address_2.index], command_1);
    }
    
    TEST_F(MemoryTest, SwapBlocksTilEndCommonCaseFirstAddressBigger) {
        using namespace lang::command;
        typedef std::initializer_list<std::tuple<lang::command::RawCommand, int>> Block;

        auto addr1 = memory->toAddress(0, 0, 10);
        auto addr2 = memory->toAddress(1, 1, 5);

        Block commandsBlock1 = { { factory::build(lang::Operator::Stop), 5 },
            { factory::build(lang::Operator::Increment, lang::Register::Register_1), 3 } };

        Block commandsBlock2 = { { factory::build(lang::Operator::Jump, 1), 5 },
            { factory::build(lang::Operator::Set, lang::Register::Register_2, 2), 3 } };


        fillWithCommands(addr1, commandsBlock1);
        fillWithCommands(addr2, commandsBlock2);

        auto result = memory->swapCellBlocksTilEnd(addr1, addr2);
        EXPECT_TRUE(result.succeed);
        EXPECT_TRUE(hasCommands(addr1, commandsBlock2));
        EXPECT_TRUE(hasCommands(addr2, commandsBlock1));
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
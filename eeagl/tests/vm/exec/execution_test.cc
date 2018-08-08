#include "gtest/gtest.h"

#include "vm/lang/command.h"
#include "vm/memory/memory.h"
#include "vm/memory/memory_dump_test.h"
#include "vm/exec/execution.h"

namespace eeagl::vm::exec {
    class ExecutionTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            auto dump = memory::MemoryDumpTest::createSimpleMemoryDump();
            memory = std::make_shared<memory::Memory>(dump);
        }
        std::shared_ptr<memory::Memory> memory;
    };

    template <typename K, typename V>
    void mapContainsAllElementsOfSet(std::set<K> set, std::unordered_map<K, V> map) {
        EXPECT_EQ(set.size(), map.size());
        for (auto item : set)
            EXPECT_NE(map.find(item), map.end());
    }

    TEST_F(ExecutionTest, ContextContainsAllDirectionRegisters) {
        Context context(*memory, memory::MemoryAddress(0, 0, lang::toPointer(0)));
        mapContainsAllElementsOfSet<lang::DirectionRegister, lang::Direction>(
            lang::DIRECTION_REGISTERS, context.directionRegisters);
    }

    TEST_F(ExecutionTest, ContextContainsAllRegisters) {
        Context context(*memory, memory::MemoryAddress(0, 0, lang::toPointer(0)));
        mapContainsAllElementsOfSet<lang::Register, lang::CellCommandPointer>(lang::REGISTERS, context.registers);
    }
}
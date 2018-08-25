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
            context = std::make_shared<Context>(*memory, memory->toAddress(0, 0, lang::toPointer(0)));
            executioner = std::make_shared<Executioner>(*context);
        }
        std::shared_ptr<memory::Memory> memory;
        std::shared_ptr<Context> context;
        std::shared_ptr<Executioner> executioner;
    };

    template <typename K, typename V>
    void mapContainsAllElementsOfSet(std::set<K> set, std::unordered_map<K, V> map) {
        EXPECT_EQ(set.size(), map.size());
        for (auto item : set)
            EXPECT_NE(map.find(item), map.end());
    }

    TEST_F(ExecutionTest, ContextContainsAllDirectionRegisters) {
        mapContainsAllElementsOfSet<lang::DirectionRegister, lang::Direction>(
            lang::DIRECTION_REGISTERS, context->directionRegisters);
    }

    TEST_F(ExecutionTest, ContextContainsAllRegisters) {
        mapContainsAllElementsOfSet<lang::Register, lang::CellCommandPointer>(
            lang::REGISTERS, context->registers);
    }

    lang::RawCommand constructCommand(lang::Operator op, lang::Register operand1Register) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reg = operand1Register;
        return command;
    }

    TEST_F(ExecutionTest, InrementSimple) {
        auto reg = lang::Register::Register_1;
        context->registers[reg] = lang::toPointer(0);
        executioner->execute(constructCommand(lang::Operator::Increment, reg));
        EXPECT_EQ(context->registers[reg], lang::toPointer(1));
    }

    TEST_F(ExecutionTest, InrementToCellSizeResultsInZero) {
        auto reg = lang::Register::Register_1;
        context->registers[reg] = lang::toPointer(lang::CELL_SIZE - 1);
        executioner->execute(constructCommand(lang::Operator::Increment, reg));
        EXPECT_EQ(context->registers[reg], lang::toPointer(0));
    }

    TEST_F(ExecutionTest, InrementAddsToContextInstructionPointer) {
        EXPECT_EQ(context->ip.index, lang::toPointer(0));
        executioner->execute(constructCommand(lang::Operator::Increment, lang::Register::Register_1));
        EXPECT_EQ(context->ip.index, lang::toPointer(1));
    }
}
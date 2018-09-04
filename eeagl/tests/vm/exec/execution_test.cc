#include "gtest/gtest.h"

#include "vm/lang/command.h"
#include "vm/memory/memory.h"
#include "vm/memory/memory_dump_test.h"
#include "vm/exec/execution.h"

#include <cstddef>

namespace eeagl::vm::exec {
    class ExecutionTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            dump = memory::MemoryDumpTest::createSimpleMemoryDump(5, 5);
            memory = std::make_shared<memory::Memory>(dump);
            context = std::make_shared<Context>(*memory, memory->toAddress(0, 0, lang::toPointer(0)));
            executioner = std::make_shared<Executioner>(*context);
        }
        std::shared_ptr<memory::MemoryDump> dump;
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

    lang::RawCommand constructCommand(lang::Operator op, lang::CellCommandPointer operand1CellPointer) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.cellCommandPointer = operand1CellPointer;
        return command;
    }

    lang::RawCommand constructCommand(lang::Operator op, lang::Reference operand1Ref, 
            lang::Reference operand2Ref, lang::CellCommandPointer operand3CellPointer) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reference = operand1Ref;
        command.operand2.reference = operand2Ref;
        command.operand3.cellCommandPointer = operand3CellPointer;
        return command;
    }

    lang::RawCommand constructCommand(lang::Operator op, lang::Register reg,
        std::byte number, lang::CellCommandPointer operand3CellPointer) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reg = reg;
        command.operand2.number = number;
        command.operand3.cellCommandPointer = operand3CellPointer;
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
        context->registers[reg] = lang::MAX_CELL_INDEX;
        executioner->execute(constructCommand(lang::Operator::Increment, reg));
        EXPECT_EQ(context->registers[reg], lang::toPointer(0));
    }

    TEST_F(ExecutionTest, InrementAddsToContextInstructionPointer) {
        EXPECT_EQ(context->ip.index, lang::toPointer(0));
        auto result = executioner->execute(constructCommand(lang::Operator::Increment, lang::Register::Register_1));
        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.index, lang::toPointer(1));
    }

    TEST_F(ExecutionTest, JumpToInvalidGreaterAddress) {
        auto expected = context->ip;
        auto result = executioner->execute(constructCommand(lang::Operator::Jump, 
            (lang::CellCommandPointer)lang::CELL_SIZE));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpToInvalidNegativeAddress) {
        auto expected = context->ip;
        auto result = executioner->execute(constructCommand(lang::Operator::Jump,
            (lang::CellCommandPointer)-1));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpToCorrectAddress) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;

        auto result = executioner->execute(constructCommand(lang::Operator::Jump, jumpPosition));
        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, jumpPosition);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRefInvalidAddress) {
        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsRef,
                { lang::Direction::Same, lang::Register::Register_1 },
                { lang::Direction::Same, lang::Register::Register_1 },
                (lang::CellCommandPointer)-1));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRefEqual) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;
        char rightIndex = 0;
        char downIndex = 1;

        context->registers[lang::Register::Register_1] = lang::toPointer(rightIndex);
        context->registers[lang::Register::Register_2] = lang::toPointer(downIndex);

        dump->cells[0][1].commands[rightIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_3 }, lang::toPointer(0));

        dump->cells[1][0].commands[downIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_3 }, lang::toPointer(0));

        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsRef,
                { lang::Direction::Right, lang::Register::Register_1 },
                { lang::Direction::Down, lang::Register::Register_2 },
                jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, jumpPosition);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRefDifferent) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;
        char rightIndex = 0;
        char downIndex = 1;

        context->registers[lang::Register::Register_1] = lang::toPointer(rightIndex);
        context->registers[lang::Register::Register_2] = lang::toPointer(downIndex);

        dump->cells[0][1].commands[rightIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_3 }, lang::toPointer(0));

        dump->cells[1][0].commands[downIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_1 }, lang::toPointer(0));

        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsRef,
                { lang::Direction::Right, lang::Register::Register_1 },
                { lang::Direction::Down, lang::Register::Register_2 },
                jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, lang::toPointer((int)ipBeforeTheCall.index + 1));
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegInvalidAddress) {
        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsReg,
                lang::Register::Register_1,
                (std::byte)0,
                (lang::CellCommandPointer)-1));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegEqual) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;

        std::byte num = (std::byte)10;
        auto reg = lang::Register::Register_1;
        context->registers[reg] = num;

        auto result = executioner->execute(
            constructCommand(lang::Operator::JumpIfEqualsReg, reg, num, jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, jumpPosition);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegDifferent) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;

        std::byte num = (std::byte)10;
        auto reg = lang::Register::Register_1;
        context->registers[reg] = num;

        auto result = executioner->execute(
            constructCommand(lang::Operator::JumpIfEqualsReg, reg, (std::byte)((int)num + 5), jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, lang::toPointer((int)ipBeforeTheCall.index + 1));
    }
}
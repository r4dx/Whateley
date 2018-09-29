#include "gtest/gtest.h"

#include "vm/lang/command.h"
#include "vm/memory/memory.h"
#include "vm/memory/memory_dump_test.h"
#include "vm/exec/execution.h"

#include <cstddef>
#include <functional>

namespace eeagl::vm::exec {
    class ExecutionTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            dump = memory::MemoryDumpTest::createSimpleMemoryDump(5, 5);
            memory = std::make_shared<memory::Memory>(dump);
            context = std::make_shared<Context>(*memory, memory->toAddress(0, 0, 0));
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

    lang::RawCommand constructCommand(lang::Operator op, lang::DirectionRegister operand1Register) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.directionReg = operand1Register;
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

    lang::RawCommand constructCommand(lang::Operator op, lang::Reference operand1Ref,
        lang::Reference operand2Ref, lang::Reference operand3Ref) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reference = operand1Ref;
        command.operand2.reference = operand2Ref;
        command.operand3.reference = operand3Ref;
        return command;
    }


    lang::RawCommand constructCommand(lang::Operator op, lang::Register reg,
        lang::Number number, lang::CellCommandPointer operand3CellPointer) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reg = reg;
        command.operand2.number = number;
        command.operand3.cellCommandPointer = operand3CellPointer;
        return command;
    }

    lang::RawCommand constructCommand(lang::Operator op, lang::Register reg, lang::Number number) {
        lang::RawCommand command;
        command.op = op;
        command.operand1.reg = reg;
        command.operand2.number = number;
        return command;
    }

    lang::RawCommand constructCommand(lang::Operator op) {
        lang::RawCommand command;
        command.op = op;
        return command;
    }

    TEST_F(ExecutionTest, InrementSimple) {
        auto reg = lang::Register::Register_1;
        context->registers[reg] = 0;
        executioner->execute(constructCommand(lang::Operator::Increment, reg));
        EXPECT_EQ(context->registers[reg], 1);
    }

    TEST_F(ExecutionTest, InrementToCellSizeResultsInZero) {
        auto reg = lang::Register::Register_1;
        context->registers[reg] = lang::MAX_CELL_INDEX;
        executioner->execute(constructCommand(lang::Operator::Increment, reg));
        EXPECT_EQ(context->registers[reg], 0);
    }

    TEST_F(ExecutionTest, JumpToInvalidGreaterAddress) {
        auto expected = context->ip;
        auto result = executioner->execute(
            constructCommand(lang::Operator::Jump, lang::CELL_SIZE));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpToInvalidNegativeAddress) {
        auto expected = context->ip;
        auto result = executioner->execute(constructCommand(lang::Operator::Jump, lang::MAX_CELL_INDEX + 1));
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
        auto expected = context->ip;
        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsRef,
                { lang::Direction::Same, lang::Register::Register_1 },
                { lang::Direction::Same, lang::Register::Register_1 },
                lang::MAX_CELL_INDEX + 1));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRefEqual) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;
        lang::Number rightIndex = 0;
        lang::Number downIndex = 1;

        context->registers[lang::Register::Register_1] = rightIndex;
        context->registers[lang::Register::Register_2] = downIndex;

        auto command = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_3 }, 0);

        dump->cells[0][1].commands[rightIndex] = command;
        dump->cells[1][0].commands[downIndex] = command;

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
        lang::Number rightIndex = 0;
        lang::Number downIndex = 1;

        context->registers[lang::Register::Register_1] = rightIndex;
        context->registers[lang::Register::Register_2] = downIndex;

        dump->cells[0][1].commands[rightIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_3 }, 0);

        dump->cells[1][0].commands[downIndex] = constructCommand(lang::Operator::JumpIfEqualsRef,
            { lang::Direction::Same, lang::Register::Register_3 },
            { lang::Direction::Same, lang::Register::Register_1 }, 0);

        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsRef,
                { lang::Direction::Right, lang::Register::Register_1 },
                { lang::Direction::Down, lang::Register::Register_2 },
                jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, ipBeforeTheCall.index + 1);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegInvalidAddress) {
        auto expected = context->ip;
        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsReg,
                lang::Register::Register_1,
                0,
                lang::MAX_CELL_INDEX + 1));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::INVALID_ADDRESS);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegNumberTooBig) {
        auto expected = context->ip;
        auto result = executioner->execute(
            constructCommand(
                lang::Operator::JumpIfEqualsReg,
                lang::Register::Register_1,
                lang::MAX_NUMBER + 1,
                0));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::NUMBER_IS_TOO_BIG);
        EXPECT_EQ(expected, context->ip);
    }

    TEST_F(ExecutionTest, JumpIfEqualsRegEqual) {
        auto ipBeforeTheCall = context->ip;
        auto jumpPosition = lang::MAX_CELL_INDEX;

        lang::Number num = 10;
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

        lang::Number num = 10;
        auto reg = lang::Register::Register_1;
        context->registers[reg] = num;

        auto result = executioner->execute(
            constructCommand(lang::Operator::JumpIfEqualsReg, reg, num + 5, jumpPosition));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip.x, ipBeforeTheCall.x);
        EXPECT_EQ(context->ip.y, ipBeforeTheCall.y);
        EXPECT_EQ(context->ip.index, ipBeforeTheCall.index + 1);
    }

    TEST_F(ExecutionTest, SetNumberTooBig) {
        auto contextBeforeTheCall = *context;

        auto reg = lang::Register::Register_1;
        auto result = executioner->execute(constructCommand(lang::Operator::Set, reg, lang::MAX_NUMBER + 1));

        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, exec::Executioner::ExecutionResult::Error::NUMBER_IS_TOO_BIG);
        EXPECT_EQ(context->registers[reg], contextBeforeTheCall.registers[reg]);
        EXPECT_EQ(context->ip, contextBeforeTheCall.ip);
    }

    TEST_F(ExecutionTest, SetNumberSuccess) {
        auto reg = lang::Register::Register_1;
        auto number = lang::MAX_NUMBER - 1;
        auto result = executioner->execute(constructCommand(lang::Operator::Set, reg, number));

        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->registers[reg], number);
    }

    TEST_F(ExecutionTest, SetRandomDirection) {
        auto reg = lang::DirectionRegister::Directional_Register_1;
        context->directionRegisters[reg] = (lang::Direction)-1;
        auto result = executioner->execute(constructCommand(lang::Operator::SetRandomDirection, reg));

        EXPECT_TRUE(result.success);
        EXPECT_NE(lang::DIRECTIONS.find(context->directionRegisters[reg]), lang::DIRECTIONS.end());
    }

    TEST_F(ExecutionTest, StopGoesToTheRight) {
        auto contextBeforeTheCall = *context;
        auto result = executioner->execute(constructCommand(lang::Operator::Stop));
        EXPECT_TRUE(result.success);
        EXPECT_EQ(context->ip, *contextBeforeTheCall.ip.neighborCell(lang::Direction::Right));
    }

    TEST_F(ExecutionTest, StopGoesDown) {
        context->ip.x = dump->cells[0].size() - 1;
        auto contextBeforeTheCall = *context;
        auto result = executioner->execute(constructCommand(lang::Operator::Stop));
        EXPECT_TRUE(result.success);
        auto expectedAddr = *contextBeforeTheCall.ip.neighborCell(lang::Direction::Down);
        expectedAddr.x = 0;
        EXPECT_EQ(context->ip, expectedAddr);
    }

    TEST_F(ExecutionTest, StopExecutionStops) {
        context->ip.x = dump->cells[0].size() - 1;
        context->ip.y = dump->cells.size() - 1;
        auto contextBeforeTheCall = *context;
        auto result = executioner->execute(constructCommand(lang::Operator::Stop));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, Executioner::ExecutionResult::END_OF_CODE);
    }

    void testSwapDereferenceError(vm::exec::Context& context, 
        Executioner& executioner, int corruptedParameterIndex, Executioner::ExecutionResult::Error expectedError) {

        EXPECT_LE(corruptedParameterIndex, 2);
        EXPECT_GE(corruptedParameterIndex, 0);
        auto expected = context.ip;
        auto corruptedReg = lang::Register::Register_1;
        auto reg = lang::Register::Register_2;
        context.registers[corruptedReg] = lang::MAX_CELL_INDEX + 1;
        auto result = executioner.execute(
            constructCommand(
                lang::Operator::SwapIfEquals,
                { lang::Direction::Same, corruptedParameterIndex == 0 ? corruptedReg : reg },
                { lang::Direction::Same, corruptedParameterIndex == 1 ? corruptedReg : reg },
                { lang::Direction::Same, corruptedParameterIndex == 2 ? corruptedReg : reg }));
        EXPECT_FALSE(result.success);
        EXPECT_EQ(result.error, expectedError);
        EXPECT_EQ(expected, context.ip);
    }

    TEST_F(ExecutionTest, SwapIfEqualsInvalidAddress1) {
        testSwapDereferenceError(*context, *executioner, 0, exec::Executioner::ExecutionResult::Error::DEREFERENCE_ERROR);
    }

    TEST_F(ExecutionTest, SwapIfEqualsInvalidAddress2) {
        testSwapDereferenceError(*context, *executioner, 1, exec::Executioner::ExecutionResult::Error::DEREFERENCE_ERROR);
    }

    TEST_F(ExecutionTest, SwapIfEqualsInvalidAddress3) {
        testSwapDereferenceError(*context, *executioner, 2, exec::Executioner::ExecutionResult::Error::SWAP_ERROR);
    }

    TEST_F(ExecutionTest, SwapIfEqualsDoesntSwapWhenDifferent) {
        auto compareAndSwapReg = lang::Register::Register_1;
        auto compareReg = lang::Register::Register_2;
        auto swapReg = lang::Register::Register_3;
        context->registers[compareAndSwapReg] = 0;
        context->registers[compareReg] = 1;
        context->registers[swapReg] = 2;

        auto expectedCompareAndSwapCommand = constructCommand(vm::lang::Operator::Stop);
        auto expectedSwapCommand = constructCommand(vm::lang::Operator::Increment,
            vm::lang::Register::Register_1);

        dump->cells[0][0].commands[context->registers[compareAndSwapReg]] = expectedCompareAndSwapCommand;
        dump->cells[0][0].commands[context->registers[compareReg]] = constructCommand(vm::lang::Operator::Jump, 1);
        dump->cells[0][0].commands[context->registers[swapReg]] = expectedSwapCommand;

        auto result = executioner->execute(
            constructCommand(
                lang::Operator::SwapIfEquals,
                { lang::Direction::Same, compareAndSwapReg },
                { lang::Direction::Same, compareReg },
                { lang::Direction::Same, swapReg }));
        EXPECT_TRUE(result.success);
        EXPECT_EQ(dump->cells[0][0].commands[context->registers[compareAndSwapReg]], expectedCompareAndSwapCommand);
        EXPECT_EQ(dump->cells[0][0].commands[context->registers[swapReg]], expectedSwapCommand);
    }

    TEST_F(ExecutionTest, SwapIfEqualsSwapsWhenEqual) {
        auto compareAndSwapReg = lang::Register::Register_1;
        auto compareReg = lang::Register::Register_2;
        auto swapReg = lang::Register::Register_3;
        context->registers[compareAndSwapReg] = 0;
        context->registers[compareReg] = 1;
        context->registers[swapReg] = 2;

        auto expectedCompareAndSwapCommand = constructCommand(vm::lang::Operator::Stop);
        auto expectedSwapCommand = constructCommand(vm::lang::Operator::Increment,
            vm::lang::Register::Register_1);

        dump->cells[0][0].commands[context->registers[compareAndSwapReg]] = expectedCompareAndSwapCommand;
        dump->cells[0][0].commands[context->registers[compareReg]] = expectedCompareAndSwapCommand;
        dump->cells[0][0].commands[context->registers[swapReg]] = expectedSwapCommand;

        auto result = executioner->execute(
            constructCommand(
                lang::Operator::SwapIfEquals,
                { lang::Direction::Same, compareAndSwapReg },
                { lang::Direction::Same, compareReg },
                { lang::Direction::Same, swapReg }));
        EXPECT_TRUE(result.success);
        EXPECT_EQ(dump->cells[0][0].commands[context->registers[compareAndSwapReg]], expectedSwapCommand);
        EXPECT_EQ(dump->cells[0][0].commands[context->registers[swapReg]], expectedCompareAndSwapCommand);
    }


    void testIPIncremented(vm::exec::Context& context, std::function<void()> action) {
        auto oldIP = context.ip;
        action();
        auto newIP = context.ip;
        EXPECT_EQ(newIP.x, oldIP.x);
        EXPECT_EQ(newIP.y, oldIP.y);
        EXPECT_EQ(newIP.index, oldIP.index + 1);
    }

    TEST_F(ExecutionTest, SetIncrementsIPOnSuccess) {
        testIPIncremented(*context, [this]() {
            executioner->execute(constructCommand(lang::Operator::Set, lang::Register::Register_1, 0));
        });
    }

    TEST_F(ExecutionTest, InrementIncrementsIPOnSuccess) {
        testIPIncremented(*context, [this]() {
            executioner->execute(constructCommand(lang::Operator::Increment, lang::Register::Register_1));
        });
    }

    TEST_F(ExecutionTest, SetRandomDirectionIncrementsIP) {
        testIPIncremented(*context, [this]() {
            executioner->execute(constructCommand(lang::Operator::SetRandomDirection, 
                lang::DirectionRegister::Directional_Register_1));
        });
    }

    TEST_F(ExecutionTest, SwapIfEqualsIncrementsIPOnEquals) {
        testIPIncremented(*context, [this]() {
            auto reg = lang::Register::Register_3;
            context->registers[reg] = 0;
            auto command = constructCommand(lang::Operator::SwapIfEquals,
                { lang::Direction::Same, reg },
                { lang::Direction::Same, reg }, 
                { lang::Direction::Same, reg });

            executioner->execute(command);
        });
    }

    TEST_F(ExecutionTest, SwapIfEqualsIncrementsIPOnNonEquals) {
        testIPIncremented(*context, [this]() {
            auto reg = lang::Register::Register_3;
            auto reg1 = lang::Register::Register_1;
            context->registers[reg] = 0;
            context->registers[reg1] = 1;
            dump->cells[0][0].commands[context->registers[reg]] = constructCommand(lang::Operator::Set, reg, 2);
            dump->cells[0][0].commands[context->registers[reg1]] = constructCommand(lang::Operator::Set, reg, 1);
            auto command = constructCommand(lang::Operator::SwapIfEquals,
                { lang::Direction::Same, reg },
                { lang::Direction::Same, reg1 },
                { lang::Direction::Same, reg });

            executioner->execute(command);
        });
    }

}
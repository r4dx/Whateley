#include "gtest/gtest.h"
#include "generator/generator.h"
#include <vector>
#include <functional>

namespace eeagl {
    namespace generator {
        class MemoryDumpGeneratorTest : public ::testing::Test {
        protected:
            virtual void SetUp() override {
            }

            GeneratorParameters emptyParams() {
                GeneratorParameters params;
                params.xDimension = 1;
                params.yDimension = 1;
                params.operators = {  };
                params.registers = {  };
                params.directionRegisters = {  };
                params.directions = {  };
                return params;
            }
        };

        TEST_F(MemoryDumpGeneratorTest, ZeroOperatorsTest) {
            GeneratorParameters params;
            params.operators.clear();
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::ZERO_OPERATORS);
        }

        TEST_F(MemoryDumpGeneratorTest, DimensionXLessThanZeroTest) {
            GeneratorParameters params;
            params.xDimension = -1;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::INVALID_DIMENSIONS);
        }

        TEST_F(MemoryDumpGeneratorTest, DimensionYLessThanZeroTest) {
            GeneratorParameters params;
            params.yDimension = -1;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::INVALID_DIMENSIONS);
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDimensionsTest) {
            GeneratorParameters params = emptyParams();
            params.xDimension = 2;
            params.yDimension = 3;
            params.operators = { vm::lang::Operator::Stop };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            ASSERT_EQ(result.result->cells.size(), params.yDimension);
            for (auto& col : result.result->cells)
                ASSERT_EQ(col.size(), params.xDimension);
        }

        TEST_F(MemoryDumpGeneratorTest, NotEnoughSlotsTest) {
            GeneratorParameters params = emptyParams();
            params.xDimension = 1;
            params.yDimension = 1;
            params.operators = vm::lang::OPERATORS;
            params.directions = vm::lang::DIRECTIONS;
            params.directionRegisters = vm::lang::DIRECTION_REGISTERS;
            params.registers = vm::lang::REGISTERS;

            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::NOT_ENOUGH_SLOTS);
        }

        void testAtLeastOneAndExpected(std::set<vm::lang::Operator> operators, vm::lang::Cell& cell,
            std::function<void(vm::lang::RawCommand)> assertion) {
            bool atLeastOne = false;
            for (auto command : cell.commands)
                if (operators.find(command.op) != operators.end()) {
                    atLeastOne = true;
                    assertion(command);
                }
            ASSERT_TRUE(atLeastOne);
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedOperatorsTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::Stop, vm::lang::Operator::Increment };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::lang::Cell cell = result.result->cells.at(0).at(0);

            for (auto command : cell.commands)
                ASSERT_NE(params.operators.find(command.op), params.operators.end());

            ASSERT_TRUE(cell.commands != 0);
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedRegistersTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::Increment };
            params.registers = { vm::lang::Register::Register_2, vm::lang::Register::Register_3 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::lang::Cell cell = result.result->cells.at(0).at(0);

            testAtLeastOneAndExpected(params.operators, cell,
                [&params](auto command) {
                ASSERT_NE(params.registers.find(command.operand1.reg), params.registers.end());
            });
        }

        TEST_F(MemoryDumpGeneratorTest, NoOperatorForOperandTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::Increment };
            params.directionRegisters = { vm::lang::DirectionRegister::Directional_Register_1 };
            params.registers = { vm::lang::Register::Register_1 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::NO_OPERATOR_FOR_OPERAND);
        }

        TEST_F(MemoryDumpGeneratorTest, NoOperandForOperatorTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::Increment };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::NO_OPERAND_FOR_OPERATOR);
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDirectionsTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::SwapIfEquals };
            params.directions = { vm::lang::Direction::Same };
            params.registers = { vm::lang::Register::Register_1 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::lang::Cell cell = result.result->cells.at(0).at(0);

            testAtLeastOneAndExpected(params.operators, cell,
                [&params](auto command) {
                ASSERT_NE(params.directions.find(command.operand1.reference.direction), params.directions.end());
            }); 
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDirectionRegistersTest) {
            GeneratorParameters params = emptyParams();
            params.operators = { vm::lang::Operator::SetRandomDirection };
            params.directionRegisters = { vm::lang::DirectionRegister::Directional_Register_1 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::lang::Cell cell = result.result->cells.at(0).at(0);

            testAtLeastOneAndExpected(params.operators, cell, 
                [&params](auto command) {
                ASSERT_NE(params.directionRegisters.find(command.operand1.directionReg), params.directionRegisters.end());
            });
        }
    }
}

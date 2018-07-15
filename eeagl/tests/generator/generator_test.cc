#include "gtest/gtest.h"
#include "generator/generator.h"
#include <vector>

namespace eeagl {
    namespace generator {
        class MemoryDumpGeneratorTest : public ::testing::Test {
        protected:
            virtual void SetUp() override {
            }

            GeneratorParameters simplestParams() {
                GeneratorParameters params;
                params.xDimension = 1;
                params.yDimension = 1;
                params.operators = { vm::command::Operator::Increment };
                params.registers = { vm::command::Register::Register_1 };
                params.directionRegisters = { vm::command::DirectionRegister::Directional_Register_1 };
                params.directions = { vm::command::Direction::Down };
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

        TEST_F(MemoryDumpGeneratorTest, ZeroDirectionsTest) {
            GeneratorParameters params;
            params.directions.clear();
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::ZERO_DIRECTIONS);
        }

        TEST_F(MemoryDumpGeneratorTest, ZeroRegistersTest) {
            GeneratorParameters params;
            params.registers.clear();
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::ZERO_REGISTERS);
        }

        TEST_F(MemoryDumpGeneratorTest, ZeroDirectionRegistersTest) {
            GeneratorParameters params;
            params.registers.clear();
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::ZERO_DIRECTION_REGISTERS);
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

        TEST_F(MemoryDumpGeneratorTest, DimensionXLargerThanMaxTest) {
            GeneratorParameters params;
            params.xDimension = vm::memory::MemoryDump::MAX_DIMENSION + 1;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::INVALID_DIMENSIONS);
        }

        TEST_F(MemoryDumpGeneratorTest, DimensionYLargerThanMaxTest) {
            GeneratorParameters params;
            params.yDimension = vm::memory::MemoryDump::MAX_DIMENSION + 1;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_FALSE(result.succeed);
            ASSERT_EQ(result.error, GenerateResult::Error::INVALID_DIMENSIONS);
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDimensionsTest) {
            GeneratorParameters params = simplestParams();
            params.xDimension = 2;
            params.xDimension = 3;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            ASSERT_EQ(result.result->cells.size(), params.xDimension);
            for (auto& col : result.result->cells)
                ASSERT_EQ(col.size(), params.yDimension);
        }


        TEST_F(MemoryDumpGeneratorTest, ExpectedOperatorsTest) {
            GeneratorParameters params = simplestParams();
            params.operators = { vm::command::Operator::Stop, vm::command::Operator::Increment };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::command::Cell cell = result.result->cells.at(0).at(0);
            for (auto command : cell.commands)
                ASSERT_NE(params.operators.find(command.op), params.operators.end());
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedRegistersTest) {
            GeneratorParameters params = simplestParams();
            params.operators = { vm::command::Operator::Increment };
            params.registers = { vm::command::Register::Register_1, vm::command::Register::Register_2 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::command::Cell cell = result.result->cells.at(0).at(0);
            for (auto command : cell.commands)
                ASSERT_NE(params.registers.find(command.operand1.reg), params.registers.end());
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDirectionsTest) {
            GeneratorParameters params = simplestParams();
            params.operators = { vm::command::Operator::Jump };
            params.directions = { vm::command::Direction::Same };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::command::Cell cell = result.result->cells.at(0).at(0);
            for (auto command : cell.commands)
                ASSERT_NE(params.directions.find(command.operand1.reference.direction), params.directions.end());
        }

        TEST_F(MemoryDumpGeneratorTest, ExpectedDirectionRegistersTest) {
            GeneratorParameters params = simplestParams();
            params.operators = { vm::command::Operator::SetRandomDirection };
            params.directionRegisters = { vm::command::DirectionRegister::Directional_Register_1 };
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            vm::command::Cell cell = result.result->cells.at(0).at(0);
            for (auto command : cell.commands)
                ASSERT_NE(params.directionRegisters.find(command.operand1.directionReg), params.directionRegisters.end());
        }
    }
}

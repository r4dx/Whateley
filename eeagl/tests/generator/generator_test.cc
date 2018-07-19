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

            GeneratorParameters simplestParams() {
                GeneratorParameters params;
                params.xDimension = 1;
                params.yDimension = 1;
                params.operators = { vm::lang::Operator::Increment };
                params.registers = { vm::lang::Register::Register_1 };
                params.directionRegisters = { vm::lang::DirectionRegister::Directional_Register_1 };
                params.directions = { vm::lang::Direction::Down };
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
            params.directionRegisters.clear();
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

        TEST_F(MemoryDumpGeneratorTest, ExpectedDimensionsTest) {
            GeneratorParameters params = simplestParams();
            params.xDimension = 2;
            params.yDimension = 3;
            MemoryDumpGenerator generator(params);
            GenerateResult result = generator.generateRandom();
            ASSERT_TRUE(result.succeed);
            ASSERT_EQ(result.result->cells.size(), params.yDimension);
            for (auto& col : result.result->cells)
                ASSERT_EQ(col.size(), params.xDimension);
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
            GeneratorParameters params = simplestParams();
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
            GeneratorParameters params = simplestParams();
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

        TEST_F(MemoryDumpGeneratorTest, ExpectedDirectionsTest) {
            GeneratorParameters params = simplestParams();
            params.operators = { vm::lang::Operator::Jump };
            params.directions = { vm::lang::Direction::Same };
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
            GeneratorParameters params = simplestParams();
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

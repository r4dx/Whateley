#pragma once
#include <set>
#include <optional>
#include "vm/lang/command_structure.h"
#include "vm/lang/command.h"
#include "vm/memory/memory_dump.h"
#include <memory>

namespace eeagl {
    namespace generator {
        struct GeneratorParameters {
            int xDimension;
            int yDimension;
            std::set<vm::lang::Direction> directions;
            std::set<vm::lang::Operator> operators;
            std::set<vm::lang::Register> registers;
            std::set<vm::lang::DirectionRegister> directionRegisters;

            GeneratorParameters();
        };

        struct GenerateResult {
            bool succeed;
            enum Error { 
                NONE, 
                ZERO_OPERATORS, 
                INVALID_DIMENSIONS,
                NO_OPERAND_FOR_OPERATOR,
                NO_OPERATOR_FOR_OPERAND } error;

            std::shared_ptr<vm::memory::MemoryDump> result;
        };

        class MemoryDumpGenerator {
        public:
            MemoryDumpGenerator(const GeneratorParameters parameters);
            GenerateResult generateRandom();

        private:
            const GeneratorParameters parameters;
            std::set<vm::lang::Direction> remainingDirections;
            std::set<vm::lang::Operator> remainingOperators;
            std::set<vm::lang::Register> remainingRegisters;
            std::set<vm::lang::DirectionRegister> remainingDirectionRegisters;

            std::optional<vm::lang::RawCommand> getCommandForOperand(vm::lang::OperandType operandType);
            std::optional<vm::lang::Operand> getRandomOperand(vm::lang::OperandType type);
            std::optional<vm::lang::RawCommand> getRandomCommand(vm::lang::CommandStructure structure);
            int getOperatorWeight(vm::lang::Operator op);
            int getOperandWeight(vm::lang::OperandType operandType);
        };
    }
}

#pragma once
#include "vm/lang/command/structure.h"
#include "vm/lang/command/command.h"
#include "vm/memory/memory_dump.h"
#include "vm/memory/memory.h"

#include <memory>
#include <tuple>
#include <set>
#include <map>
#include <vector>
#include <optional>
#include <random>

namespace eeagl::generator {
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
            NO_OPERATOR_FOR_OPERAND,
            NOT_ENOUGH_SLOTS
        } error;

        std::shared_ptr<vm::memory::MemoryDump> result;
    };

    class MemoryDumpGenerator {
    public:
        MemoryDumpGenerator(const GeneratorParameters parameters);
        GenerateResult generateRandom();

    private:
        std::random_device device;
        std::mt19937 engine;

        const GeneratorParameters parameters;
        std::set<vm::lang::Direction> remainingDirections;
        std::set<vm::lang::Operator> remainingOperators;
        std::set<vm::lang::Register> remainingRegisters;
        std::set<vm::lang::DirectionRegister> remainingDirectionRegisters;
        std::vector<vm::lang::command::RawCommand> commandsToAdd;

        unsigned int commandSlots;

        std::optional<vm::lang::command::RawCommand> getCommandForOperand(vm::lang::command::OperandType operandType);
        std::optional<vm::lang::Operand> getRandomOperand(vm::lang::command::OperandType type);
        std::optional<vm::lang::command::RawCommand> getRandomCommand(vm::lang::command::CommandStructure structure);
        int getOperatorWeight(vm::lang::Operator op);
        int getOperandWeight(vm::lang::command::OperandType operandType);

        template <typename T>
        bool addCommandsForOperandUntilNonRemaining(std::set<T>& set,
            std::optional<vm::lang::command::OperandType> operandType = std::nullopt);
    };
}

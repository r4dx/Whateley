#pragma once
#include "generator.h"
#include "util/random.h"

#include <memory>
#include <map>
#include <iterator>
#include <numeric>
#include "optional"

namespace eeagl::generator {
    template <typename T>
    std::optional<T> getRandom(std::set<T>& remainingSet, const std::set<T>& fullSet) {
        if (remainingSet.size() > 0)
            return util::random::popRandom<T>(remainingSet);
        return util::random::random<T>(fullSet);
    }

    GeneratorParameters::GeneratorParameters() {
        xDimension = 5;
        yDimension = 5;
        directions = vm::lang::DIRECTIONS;
        operators = vm::lang::OPERATORS;
        registers = vm::lang::REGISTERS;
        directionRegisters = vm::lang::DIRECTION_REGISTERS;
    }

    MemoryDumpGenerator::MemoryDumpGenerator(const GeneratorParameters parameters) : parameters(parameters), 
        engine(device()) { }

    template <typename T>
    bool MemoryDumpGenerator::addCommandsForOperandUntilNonRemaining(std::set<T>& set, 
        std::optional<vm::lang::command::OperandType> operandType) {

        if (!operandType.has_value())
            operandType = vm::lang::command::OperandToOperandType<T>();

        while (set.size() > 0) {
            auto command = getCommandForOperand(*operandType);
            if (!command.has_value()) {
                return false;
            }
            commandsToAdd.push_back(*command);
        }
        return true;
    }

    GenerateResult MemoryDumpGenerator::generateRandom() {
        GenerateResult result;
        result.succeed = false;

        if (parameters.xDimension <= 0 || parameters.yDimension <= 0) {
            result.error = GenerateResult::Error::INVALID_DIMENSIONS;
            return result;
        }

        if (parameters.operators.size() <= 0) {
            result.error = GenerateResult::Error::ZERO_OPERATORS;
            return result;
        }

        result.result = std::make_shared<vm::memory::MemoryDump>();
        result.result->header.signature = vm::memory::MemoryDumpHeader::SIGNATURE;
        result.result->header.version = vm::memory::MemoryDumpHeader::CURRENT_VERSION;
            
        result.result->cells = std::vector< std::vector < vm::lang::command::Cell > >(parameters.yDimension, 
            std::vector<vm::lang::command::Cell>(parameters.xDimension, vm::lang::command::Cell()));

        commandSlots = parameters.yDimension * parameters.xDimension * vm::lang::CELL_SIZE;

        remainingDirections = parameters.directions;
        remainingOperators = parameters.operators;
        remainingRegisters = parameters.registers;
        remainingDirectionRegisters = parameters.directionRegisters;
        commandsToAdd.clear();

        while (remainingOperators.size() > 0) {
            auto commandOp = *(getRandom<vm::lang::Operator>(remainingOperators, parameters.operators));
            auto structure = vm::lang::command::COMMAND_STRUCTURE.at(commandOp);
            auto commandResult = getRandomCommand(structure);
            if (!commandResult.has_value()) {
                result.error = GenerateResult::Error::NO_OPERAND_FOR_OPERATOR;
                return result;
            }
            commandsToAdd.push_back(*commandResult);
        }

        if (!addCommandsForOperandUntilNonRemaining<vm::lang::Register>(remainingRegisters) || 
            !addCommandsForOperandUntilNonRemaining<vm::lang::DirectionRegister>(remainingDirectionRegisters) ||
            !addCommandsForOperandUntilNonRemaining<vm::lang::Direction>(remainingDirections, 
                vm::lang::command::OperandType::TypeReference)) {

            result.error = GenerateResult::Error::NO_OPERATOR_FOR_OPERAND;
            return result;
        }

        if (commandsToAdd.size() > commandSlots) {
            result.error = GenerateResult::Error::NOT_ENOUGH_SLOTS;
            return result;
        }

        std::vector<int> unusedIndices = std::vector<int>(commandSlots);
        std::iota(unusedIndices.begin(), unusedIndices.end(), 0);
        std::shuffle(unusedIndices.begin(), unusedIndices.end(), engine);
        size_t firstUnusedIndex = 0;

        while (commandsToAdd.size() > 0) {
            int flatCoords = unusedIndices.at(firstUnusedIndex++);
            auto coords = vm::memory::MemoryAddress::fromFlatIndex(flatCoords, parameters.xDimension, 
				parameters.yDimension, vm::lang::CELL_SIZE);
            vm::lang::command::RawCommand command = commandsToAdd[0];
            commandsToAdd.erase(commandsToAdd.begin());
            result.result->cells[coords.y][coords.x].commands[coords.index] = command;
        }

        while (firstUnusedIndex < unusedIndices.size()) {
            int flatCoords = unusedIndices.at(firstUnusedIndex++);
            auto coords = vm::memory::MemoryAddress::fromFlatIndex(flatCoords, parameters.xDimension, 
				parameters.yDimension, vm::lang::CELL_SIZE);
            auto commandOp = *(util::random::random<vm::lang::Operator>(parameters.operators));
            auto commandResult = getRandomCommand(vm::lang::command::COMMAND_STRUCTURE.at(commandOp));
            if (!commandResult.has_value()) {
                result.error = GenerateResult::Error::NO_OPERAND_FOR_OPERATOR;
                return result;
            }
            result.result->cells[coords.y][coords.x].commands[coords.index] = *commandResult;
        }

        result.succeed = true;
        return result;
    }

    std::optional<vm::lang::command::RawCommand> MemoryDumpGenerator::getRandomCommand(vm::lang::command::CommandStructure structure) {
        auto operand1Result = getRandomOperand(structure.operand1);
        auto operand2Result = getRandomOperand(structure.operand2);
        auto operand3Result = getRandomOperand(structure.operand3);
        if (!operand1Result.has_value() || !operand2Result.has_value() || !operand3Result.has_value()) {
            return std::nullopt;
        }
        vm::lang::command::RawCommand command;
        command.op = structure.op;
        command.operand1 = *operand1Result;
        command.operand2 = *operand2Result;
        command.operand3 = *operand3Result;
        return command;
    }

    std::optional<vm::lang::Operand> MemoryDumpGenerator::getRandomOperand(vm::lang::command::OperandType type) {
        vm::lang::Operand result;
        result.number = 0;

        std::uniform_int_distribution<int> pointerUD(0, vm::lang::CELL_SIZE - 1);
        std::uniform_int_distribution<int> numberUD(vm::lang::MIN_NUMBER, vm::lang::MAX_NUMBER - 1);

        switch (type) {
        case vm::lang::command::OperandType::TypeCellCommandPointer: {
            result.cellCommandPointer = pointerUD(engine);
            break;
        }
        case vm::lang::command::OperandType::TypeDirectionRegister: {
            auto dirReg = getRandom<vm::lang::DirectionRegister>(
                remainingDirectionRegisters, parameters.directionRegisters);
            if (!dirReg.has_value())
                return std::nullopt;

            result.directionReg = *dirReg;
            break;
        }
        case vm::lang::command::OperandType::TypeNumber: {
            result.number = numberUD(engine);
            break;
        }
        case vm::lang::command::OperandType::TypeReference: {
            auto dir = getRandom<vm::lang::Direction>(remainingDirections, parameters.directions);
            if (!dir.has_value())
                return std::nullopt;

            result.reference.direction = *dir;
            auto reg = getRandom<vm::lang::Register>(remainingRegisters, parameters.registers);
            if (!reg.has_value())
                return std::nullopt;

            result.reference.reg = *reg;
            break;
        }
        case vm::lang::command::OperandType::TypeRegister: {
            auto randomReg = getRandom<vm::lang::Register>(remainingRegisters, parameters.registers);
            if (!randomReg.has_value())
                return std::nullopt;

            result.reg = *randomReg;
            break;
        }
        }

        return result;
    }

    int MemoryDumpGenerator::getOperatorWeight(vm::lang::Operator op) {
        auto structure = vm::lang::command::COMMAND_STRUCTURE.at(op);
        return getOperandWeight(structure.operand1) + getOperandWeight(structure.operand2) +
            getOperandWeight(structure.operand3);
    }

    int MemoryDumpGenerator::getOperandWeight(vm::lang::command::OperandType operandType) {
        switch (operandType) {
        case vm::lang::command::OperandType::TypeDirectionRegister:
            return remainingDirectionRegisters.size();
        case vm::lang::command::OperandType::TypeReference:
            return remainingDirections.size() + remainingRegisters.size();
        case vm::lang::command::OperandType::TypeRegister:
            return remainingRegisters.size();
        }
        return 0;
    }

    std::optional<vm::lang::command::RawCommand> MemoryDumpGenerator::getCommandForOperand(vm::lang::command::OperandType operandType) {
        std::multimap<int, vm::lang::command::CommandStructure> weightedCommandStructure;
        for (auto op : parameters.operators)
            weightedCommandStructure.insert(
                std::make_pair(getOperatorWeight(op), vm::lang::command::COMMAND_STRUCTURE.at(op)));
            
        auto it = weightedCommandStructure.rbegin();
        while (it != weightedCommandStructure.rend()) {
            auto structure = it->second;

            if (structure.operand1 == operandType ||
                structure.operand2 == operandType ||
                structure.operand3 == operandType ||
                (operandType == vm::lang::command::OperandType::TypeRegister && (
                    structure.operand1 == vm::lang::command::OperandType::TypeReference ||
                    structure.operand2 == vm::lang::command::OperandType::TypeReference ||
                    structure.operand3 == vm::lang::command::OperandType::TypeReference
                    ))) {
                auto command = getRandomCommand(structure);
                if (!command.has_value())
                    continue;
                return command;
            }
            it++;
        }
        return std::nullopt;
    }
}

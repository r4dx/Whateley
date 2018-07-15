#pragma once
#include "generator.h"

namespace eeagl {
    namespace generator {

        GeneratorParameters::GeneratorParameters() {
            xDimension = 5;
            yDimension = 5;
            directions = vm::command::DIRECTIONS;
            operators = vm::command::OPERATORS;
            registers = vm::command::REGISTERS;
            directionRegisters = vm::command::DIRECTION_REGISTERS;
        }

        MemoryDumpGenerator::MemoryDumpGenerator(const GeneratorParameters parameters) : parameters(parameters) { }

        GenerateResult MemoryDumpGenerator::generateRandom() {
            GenerateResult result;
            result.succeed = false;
            return result;
        }
    }
}

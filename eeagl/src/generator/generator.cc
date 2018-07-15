#pragma once
#include "generator.h"

namespace eeagl {
    namespace generator {

        GeneratorParameters::GeneratorParameters() {
            xDimension = 5;
            yDimension = 5;
            directions = vm::lang::DIRECTIONS;
            operators = vm::lang::OPERATORS;
            registers = vm::lang::REGISTERS;
            directionRegisters = vm::lang::DIRECTION_REGISTERS;
        }

        MemoryDumpGenerator::MemoryDumpGenerator(const GeneratorParameters parameters) : parameters(parameters) { }

        GenerateResult MemoryDumpGenerator::generateRandom() {
            GenerateResult result;
            result.succeed = false;
            return result;
        }
    }
}

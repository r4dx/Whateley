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

            if (parameters.xDimension <= 0 || parameters.yDimension <= 0) {
                result.error = GenerateResult::Error::INVALID_DIMENSIONS;
                return result;
            }

            if (parameters.directionRegisters.size() <= 0) {
                result.error = GenerateResult::Error::ZERO_DIRECTION_REGISTERS;
                return result;
            }
                
            if (parameters.directions.size() <= 0) {
                result.error = GenerateResult::Error::ZERO_DIRECTIONS;
                return result;
            }

            if (parameters.operators.size() <= 0) {
                result.error = GenerateResult::Error::ZERO_OPERATORS;
                return result;
            }

            if (parameters.registers.size() <= 0) {
                result.error = GenerateResult::Error::ZERO_REGISTERS;
                return result;
            }

            result.succeed = true;
            return result;
        }
    }
}

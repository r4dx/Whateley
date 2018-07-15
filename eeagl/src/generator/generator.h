#pragma once
#include <set>
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
                ZERO_DIRECTIONS, 
                ZERO_OPERATORS, 
                ZERO_REGISTERS, 
                ZERO_DIRECTION_REGISTERS, 
                INVALID_DIMENSIONS } error;

            std::shared_ptr<vm::memory::MemoryDump> result;
        };

        class MemoryDumpGenerator {
        public:
            MemoryDumpGenerator(const GeneratorParameters parameters);
            GenerateResult generateRandom();

        private:
            const GeneratorParameters parameters;
        };
    }
}

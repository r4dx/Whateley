#pragma once

#include "vm/memory/memory.h"
#include "vm/lang/command.h"

#include <unordered_map>
#include <functional>

namespace eeagl::vm::exec {
    struct Context {
        memory::Memory& memory;

        memory::MemoryAddress ip;
        std::unordered_map<lang::Register, lang::CellCommandPointer> registers;
        std::unordered_map<lang::DirectionRegister, lang::Direction> directionRegisters;

        Context(memory::Memory& memory, memory::MemoryAddress ip);
    };

    class Executioner {
    public:
        struct ExecutionResult {
            bool success;
            enum Error {
                UNKNOWN_OPERATOR
            } error;
        };

        Executioner(Context& context);
                
        ExecutionResult execute(const lang::RawCommand& command);

    private:
        Context& context;

        std::unordered_map<lang::Operator, std::function<ExecutionResult(const lang::RawCommand&)>> bindings;

        template<lang::Operator OP>
        void bindExecute();

        template <lang::Operator Operator>
        ExecutionResult execute(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Increment>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Jump>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::JumpIfEqualsRef>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::JumpIfEqualsReg>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Set>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::SetRandomDirection>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Stop>(const lang::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::SwapIfEquals>(const lang::RawCommand& command);
    };
}
#pragma once

#include "vm/memory/memory.h"
#include "vm/lang/command/command.h"

#include <unordered_map>
#include <functional>

namespace eeagl::vm::exec {
    struct Context {
        memory::Memory& memory;

        memory::MemoryAddress ip;
        std::unordered_map<lang::Register, lang::Number> registers;
        std::unordered_map<lang::DirectionRegister, lang::Direction> directionRegisters;

        Context(memory::Memory& memory, memory::MemoryAddress ip);
    };

    class Executioner {
    public:
        struct ExecutionResult {
            bool success;
            enum Error {
                INVALID_ADDRESS,
                UNKNOWN_OPERATOR,
                DEREFERENCE_ERROR,
                SWAP_ERROR,
                NUMBER_IS_TOO_BIG,
                END_OF_CODE
            } error;

        };

        Executioner(Context& context);
                
        ExecutionResult execute(const lang::command::RawCommand& command);

    private:
        Context& context;

        std::unordered_map<lang::Operator, std::function<ExecutionResult(const lang::command::RawCommand&)>> bindings;

        memory::MemoryAddress getAddress(lang::Reference reference);

        template<lang::Operator OP>
        void bindExecute();

        template <lang::Operator Operator>
        ExecutionResult execute(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Increment>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Jump>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::JumpIfEqualsRef>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::JumpIfEqualsReg>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Set>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::SetRandomDirection>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::Stop>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::SwapIfEquals>(const lang::command::RawCommand& command);

        template <>
        ExecutionResult execute<lang::Operator::SwapCell>(const lang::command::RawCommand& command);
    };
}
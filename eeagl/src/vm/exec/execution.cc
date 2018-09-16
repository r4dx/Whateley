#include "execution.h"
#include "vm/lang/command.h"
#include "util/random.h"
#include "vm/lang/types.h"

#include <unordered_map>

namespace eeagl::vm::exec {
    Context::Context(memory::Memory& memory, memory::MemoryAddress ip) : memory(memory), ip(ip) {
        for (auto reg : lang::REGISTERS)
            registers[reg] = lang::MIN_NUMBER;

        for (auto dirReg : lang::DIRECTION_REGISTERS)
            directionRegisters[dirReg] = lang::Direction::Same;
    }

    template<lang::Operator OP>
    void Executioner::bindExecute() {
        bindings[OP] = std::bind(&Executioner::execute<OP>, this, std::placeholders::_1);
    }

    Executioner::Executioner(Context& context) : 
        context(context) {
        bindExecute<lang::Operator::Increment>();
        bindExecute<lang::Operator::Jump>();
        bindExecute<lang::Operator::JumpIfEqualsRef>();
        bindExecute<lang::Operator::JumpIfEqualsReg>();
        bindExecute<lang::Operator::Set>();
        bindExecute<lang::Operator::SetRandomDirection>();
        bindExecute<lang::Operator::Stop>();
        bindExecute<lang::Operator::SwapIfEquals>();
    }

    template <lang::Operator Operator>
    Executioner::ExecutionResult Executioner::execute(const lang::RawCommand& command) {
        return { false, Executioner::ExecutionResult::Error::UNKNOWN_OPERATOR };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::Increment>(const lang::RawCommand& command) {
        lang::Number value = context.registers[command.operand1.reg];
        value++;
        if (value > lang::MAX_CELL_INDEX)
            value = 0;

        context.registers[command.operand1.reg] = value;
        ++context.ip;
        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::Jump>(const lang::RawCommand& command) {
        if (command.operand1.cellCommandPointer > lang::MAX_CELL_INDEX)
            return { false, ExecutionResult::Error::INVALID_ADDRESS };

        context.ip.index = command.operand1.cellCommandPointer;

        return { true };
    }

    memory::MemoryAddress Executioner::getAddress(lang::Reference reference) {
        auto cell = context.ip.neighborCell(reference.direction);
        return context.memory.toAddress(cell->x, cell->y, context.registers[reference.reg]);
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::JumpIfEqualsRef>(const lang::RawCommand& command) {
        if (command.operand3.cellCommandPointer > lang::MAX_CELL_INDEX)
            return { false, ExecutionResult::Error::INVALID_ADDRESS };
        
        auto dereferenceResult = context.memory.dereference(getAddress(command.operand1.reference));
        if (!dereferenceResult.succeed)
            return { false, ExecutionResult::Error::DEREFERENCE_ERROR };
        auto op1Value = dereferenceResult.value;

        dereferenceResult = context.memory.dereference(getAddress(command.operand2.reference));
        if (!dereferenceResult.succeed)
            return { false, ExecutionResult::Error::DEREFERENCE_ERROR };
        auto op2Value = dereferenceResult.value;

        if (op1Value == op2Value)
            context.ip.index = command.operand3.cellCommandPointer;
        else
            ++context.ip;

        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::JumpIfEqualsReg>(
        const lang::RawCommand& command) {

        if (command.operand3.cellCommandPointer > lang::MAX_CELL_INDEX)
            return { false, ExecutionResult::Error::INVALID_ADDRESS };

        if (command.operand2.number > lang::MAX_NUMBER)
            return { false, ExecutionResult::Error::NUMBER_IS_TOO_BIG };

        auto regValue = context.registers[command.operand1.reg];

        if (regValue == command.operand2.number)
            context.ip.index = command.operand3.cellCommandPointer;
        else
            ++context.ip;

        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::Set>(
        const lang::RawCommand& command) {

        if (command.operand2.number > lang::MAX_NUMBER)
            return { false, ExecutionResult::Error::NUMBER_IS_TOO_BIG };

        context.registers[command.operand1.reg] = command.operand2.number;

        ++context.ip;
        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::SetRandomDirection>(
        const lang::RawCommand& command) {
        context.directionRegisters[command.operand1.directionReg] = *util::random::random(lang::DIRECTIONS);
        ++context.ip;
        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::Stop>(const lang::RawCommand& command) {
        context.ip = *context.ip.neighborCell(lang::Direction::Right);
        if (context.ip.x == 0) {
            context.ip = *context.ip.neighborCell(lang::Direction::Down);
            if (context.ip.y == 0)
                return { false, Executioner::ExecutionResult::END_OF_CODE };
        }
        return { true };
    }

    template <>
    Executioner::ExecutionResult Executioner::execute<lang::Operator::SwapIfEquals>(const lang::RawCommand& command) {
        return { true };
    }

    Executioner::ExecutionResult Executioner::execute(const lang::RawCommand& command) {
        if (bindings.find(command.op) == bindings.end())
            return { false, Executioner::ExecutionResult::Error::UNKNOWN_OPERATOR };

        auto result = bindings[command.op](command);

        return result;
    }
}
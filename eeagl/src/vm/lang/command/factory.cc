#include "factory.h"

#include <vector>

namespace eeagl::vm::lang::command::factory {
    RawCommand build(lang::Operator op, lang::Register operand1Register) {
        RawCommand command;
        command.op = op;
        command.operand1.reg = operand1Register;
        return command;
    }

    RawCommand build(lang::Operator op, lang::DirectionRegister operand1Register) {
        RawCommand command;
        command.op = op;
        command.operand1.directionReg = operand1Register;
        return command;
    }

    RawCommand build(lang::Operator op, lang::CellCommandPointer operand1CellPointer) {
        RawCommand command;
        command.op = op;
        command.operand1.cellCommandPointer = operand1CellPointer;
        return command;
    }

    RawCommand build(lang::Operator op, lang::Reference operand1Ref,
        lang::Reference operand2Ref, lang::CellCommandPointer operand3CellPointer) {
        RawCommand command;
        command.op = op;
        command.operand1.reference = operand1Ref;
        command.operand2.reference = operand2Ref;
        command.operand3.cellCommandPointer = operand3CellPointer;
        return command;
    }

    RawCommand build(lang::Operator op, lang::Reference operand1Ref,
        lang::Reference operand2Ref, lang::Reference operand3Ref) {
        RawCommand command;
        command.op = op;
        command.operand1.reference = operand1Ref;
        command.operand2.reference = operand2Ref;
        command.operand3.reference = operand3Ref;
        return command;
    }


    RawCommand build(lang::Operator op, lang::Register reg,
        lang::Number number, lang::CellCommandPointer operand3CellPointer) {
        RawCommand command;
        command.op = op;
        command.operand1.reg = reg;
        command.operand2.number = number;
        command.operand3.cellCommandPointer = operand3CellPointer;
        return command;
    }

    RawCommand build(lang::Operator op, lang::Register reg, lang::Number number) {
        RawCommand command;
        command.op = op;
        command.operand1.reg = reg;
        command.operand2.number = number;
        return command;
    }

    RawCommand build(lang::Operator op) {
        RawCommand command;
        command.op = op;
        return command;
    }
}
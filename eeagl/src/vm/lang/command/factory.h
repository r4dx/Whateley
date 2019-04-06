#pragma once
#include "command.h"

namespace eeagl::vm::lang::command::factory {
    RawCommand build(lang::Operator op, lang::Register operand1Register);
    RawCommand build(lang::Operator op, lang::DirectionRegister operand1Register);
    RawCommand build(lang::Operator op, lang::DirectionRegister operand1Register, lang::Register operand2Register);
    RawCommand build(lang::Operator op, lang::CellCommandPointer operand1CellPointer);
    RawCommand build(lang::Operator op, lang::Reference operand1Ref,
        lang::Reference operand2Ref, lang::CellCommandPointer operand3CellPointer);
    RawCommand build(lang::Operator op, lang::Reference operand1Ref,
        lang::Reference operand2Ref, lang::Reference operand3Ref);
    RawCommand build(lang::Operator op, lang::Register reg,
        lang::Number number, lang::CellCommandPointer operand3CellPointer);
    RawCommand build(lang::Operator op, lang::Register reg, lang::Number number);
    RawCommand build(lang::Operator op);
}
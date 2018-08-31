#include "command_structure.h"

namespace eeagl::vm::lang {

    int operandChecksum(OperandType type, Operand operand) {
        switch (type) {
        case TypeNumber:
            return (int)operand.number;
        case TypeRegister:
            return operand.reg;
        case TypeDirectionRegister:
            return operand.directionReg;
        case TypeCellCommandPointer:
            return (int)operand.cellCommandPointer;
        case TypeReference:
            return operand.reference.direction * 100 + operand.reference.reg;
        }
        return -1;
    }

    int operandChecksum(Operator op, int argumentNum, Operand operand) {
        switch (argumentNum) {
        case 0:
            return operandChecksum(COMMAND_STRUCTURE.at(op).operand1, operand);
        case 1:
            return operandChecksum(COMMAND_STRUCTURE.at(op).operand2, operand);
        case 2:
            return operandChecksum(COMMAND_STRUCTURE.at(op).operand3, operand);
        }
        return -1;
    }
}
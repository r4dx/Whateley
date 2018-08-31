#pragma once

#include "types.h"
#include "command_structure.h"

namespace eeagl::vm::lang {
    struct RawCommand {
        Operator op;
        Operand operand1;
        Operand operand2;
        Operand operand3;

        inline bool operator==(const RawCommand& rhs) const {
            return op == rhs.op &&
                operandChecksum(op, 0, operand1) == operandChecksum(op, 0, rhs.operand1) &&
                operandChecksum(op, 1, operand2) == operandChecksum(op, 1, rhs.operand2) &&
                operandChecksum(op, 2, operand3) == operandChecksum(op, 2, rhs.operand3);
        }
        inline bool operator!=(const RawCommand& rhs) const {
            return !this->operator==(rhs);
        }
    };
    struct Cell {
        RawCommand commands[CELL_SIZE];
    };
}
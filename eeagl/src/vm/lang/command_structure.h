#pragma once
#include "command.h"
#include <unordered_map>
namespace eeagl {
    namespace vm {
        namespace lang {
            enum OperandType {
                NonExistent,
                TypeNumber,
                TypeRegister,
                TypeDirectionRegister,
                TypeCellCommandPointer,
                TypeReference
            };

            struct CommandStructure {
                Operator op;
                OperandType operand1;
                OperandType operand2;
                OperandType operand3;
            };

            static const std::unordered_map<Operator, CommandStructure> COMMAND_STRUCTURE = {
                { Set, { Set, TypeRegister, TypeNumber, NonExistent } },
                { Increment,{ Increment, TypeRegister, NonExistent, NonExistent } },
                { JumpIfEqualsRef,{ JumpIfEqualsRef, TypeReference, TypeReference, TypeCellCommandPointer } },
                { JumpIfEqualsReg, { JumpIfEqualsReg, TypeRegister, TypeNumber, TypeCellCommandPointer } },
                { Jump, { Jump, TypeCellCommandPointer, NonExistent, NonExistent } },
                { SetRandomDirection, { SetRandomDirection, TypeDirectionRegister, NonExistent, NonExistent } },
                { SwapIfEquals, { SwapIfEquals, TypeReference, TypeReference, TypeReference } },
                { Stop, { Stop, NonExistent, NonExistent, NonExistent } }
            };
        }
    }
}
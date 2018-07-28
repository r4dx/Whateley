#pragma once
#include "command.h"
#include <unordered_map>
#include <type_traits>

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

            template <typename T>
            OperandType OperandToOperandType() {
                if constexpr(std::is_same<T, Register>::value)
                    return OperandType::TypeRegister;

                else if constexpr(std::is_same<T, DirectionRegister>::value)
                    return OperandType::TypeDirectionRegister;

                else if constexpr(std::is_same<T, Reference>::value)
                    return OperandType::TypeReference;

                else if constexpr(std::is_same<T, std::byte>::value)
                    return OperandType::TypeNumber;

                else if constexpr(std::is_same<T, CellCommandPointer>::value)
                    return OperandType::TypeCellCommandPointer;
                else
                    return OperandType::NonExistent;
            };


            template <OperandType T>
            struct OperandTypeToOperand {
                using type = void;
            };

            template <>
            struct OperandTypeToOperand<OperandType::TypeRegister> {
                using type = Register;
            };

            template <>
            struct OperandTypeToOperand<OperandType::TypeDirectionRegister> {
                using type = DirectionRegister;
            };

            template <>
            struct OperandTypeToOperand<OperandType::TypeReference> {
                using type = Reference;
            };

            template <>
            struct OperandTypeToOperand<OperandType::TypeCellCommandPointer> {
                using type = CellCommandPointer;
            };

            template <>
            struct OperandTypeToOperand<OperandType::TypeNumber> {
                using type = std::byte;
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
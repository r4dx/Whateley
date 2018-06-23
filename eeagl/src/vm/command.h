#pragma once

#include <cstddef>

namespace eeagle {
    namespace vm {
        namespace command {
            enum Operator : unsigned char {
                Set,
                Increment,
                JumpIfEquals,
                Jump,
                SetRandomDirection,
                SwapIfEquals,
                Stop
            };

            enum Register : unsigned char {
                Register_1,
                Register_2,
                Register_3,
            };

            enum DirectionRegister : unsigned char {
                Directional_Register_1
            };

            typedef std::byte CellAddress;

            enum Direction : unsigned char {
                Up,
                Down,
                Left,
                Right,
                Same
            };

            struct Reference {
                Direction direction;
                Register reg;
            };

            union Operand {
                std::byte number;
                Register reg;
                DirectionRegister directionReg;
                CellAddress cellAddress;
                Reference reference;
            };

            struct RawCommand {
                Operator op;
                Operand operand1;
                Operand operand2;
                Operand operand3;
            };

        }
    }
}
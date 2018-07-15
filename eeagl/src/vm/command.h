#pragma once

#include <cstddef>
#include <set>

namespace eeagl {
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

            static const std::set<Operator> OPERATORS = { 
                Operator::Set, 
                Operator::Increment,
                Operator::JumpIfEquals,
                Operator::Jump,
                Operator::SetRandomDirection,
                Operator::SwapIfEquals,
                Operator::Stop,
            };

            enum Register : unsigned char {
                Register_1,
                Register_2,
                Register_3,
            };

            static const std::set<Register> REGISTERS = {
                Register::Register_1,
                Register::Register_2,
                Register::Register_3
            };

            enum DirectionRegister : unsigned char {
                Directional_Register_1
            };

            static const std::set<DirectionRegister> DIRECTION_REGISTERS = {
                DirectionRegister::Directional_Register_1
            };


            typedef std::byte CellCommandPointer;

            enum Direction : unsigned char {
                Up,
                Down,
                Left,
                Right,
                Same
            };

            static const std::set<Direction> DIRECTIONS = {
                Direction::Up,
                Direction::Down,
                Direction::Left,
                Direction::Right,
                Direction::Same,
            };

            struct Reference {
                Direction direction;
                Register reg;
            };

            union Operand {
                std::byte number;
                Register reg;
                DirectionRegister directionReg;
                CellCommandPointer cellCommandPointer;
                Reference reference;
            };

            struct RawCommand {
                Operator op;
                Operand operand1;
                Operand operand2;
                Operand operand3;
            };
            static const int CELL_SIZE = 40;
            struct Cell {
                RawCommand commands[CELL_SIZE];
            };

        }
    }
}
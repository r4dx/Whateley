#pragma once

#include <cstddef>
#include <set>
#include <cassert>

namespace eeagl {
    namespace vm {
        namespace lang {
            static const int CELL_SIZE = 40;
            static const int MIN_NUMBER = 0;
            static const int MAX_NUMBER = CELL_SIZE;

            enum Operator : char {
                Set,
                Increment,
                JumpIfEqualsRef,
                JumpIfEqualsReg,
                Jump,
                SetRandomDirection,
                SwapIfEquals,
                Stop
            };

            static const std::set<Operator> OPERATORS = { 
                Operator::Set, 
                Operator::Increment,
                Operator::JumpIfEqualsRef,
                Operator::JumpIfEqualsReg,
                Operator::Jump,
                Operator::SetRandomDirection,
                Operator::SwapIfEquals,
                Operator::Stop,
            };

            enum Register : char {
                Register_1,
                Register_2,
                Register_3,
            };

            static const std::set<Register> REGISTERS = {
                Register::Register_1,
                Register::Register_2,
                Register::Register_3
            };

            enum DirectionRegister : char {
                Directional_Register_1
            };

            static const std::set<DirectionRegister> DIRECTION_REGISTERS = {
                DirectionRegister::Directional_Register_1
            };

            typedef std::byte CellCommandPointer;

            inline CellCommandPointer toPointer(char value) {
                assert(value < CELL_SIZE && value >= 0);
                return (CellCommandPointer)value;
            };

            enum Direction : char {
                Same,
                Up,
                Down,
                Left,
                Right
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
            struct Cell {
                RawCommand commands[CELL_SIZE];
            };

        }
    }
}
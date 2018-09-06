#pragma once

#include <cstddef>
#include <set>
#include <cassert>

namespace eeagl::vm::lang {
    static const int CELL_SIZE = 40;

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

    typedef int CellCommandPointer;
    typedef CellCommandPointer Number;

    static const Number MIN_NUMBER(0);
    static const Number MAX_NUMBER(CELL_SIZE - 1);

    static const CellCommandPointer MAX_CELL_INDEX(CELL_SIZE - 1);

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
        Number number;
        Register reg;
        DirectionRegister directionReg;
        CellCommandPointer cellCommandPointer;
        Reference reference;
    };
}
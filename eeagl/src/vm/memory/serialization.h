#pragma once
#include <stdexcept>

#include "vm/lang/command/command.h"
#include "vm/lang/command/structure.h"
#include "memory_dump.h"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

    template<class Archive>
    void archiveOperand(Archive& archive, eeagl::vm::lang::command::OperandType opType, eeagl::vm::lang::Operand& operand) {
        using namespace eeagl::vm::lang;

        switch (opType) {
        case eeagl::vm::lang::command::OperandType::NonExistent:
            break;
        case eeagl::vm::lang::command::OperandType::TypeCellCommandPointer:
            archive(operand.cellCommandPointer);
            break;
        case eeagl::vm::lang::command::OperandType::TypeDirectionRegister:
            archive(operand.directionReg);
            break;
        case eeagl::vm::lang::command::OperandType::TypeNumber:
            archive(operand.number);
            break;
        case eeagl::vm::lang::command::OperandType::TypeReference:
            archive(operand.reference);
            break;
        case eeagl::vm::lang::command::OperandType::TypeRegister:
            archive(operand.reg);
            break;
        default:
            throw std::exception("Serialization error: Unknown operand type");
        }
    }

    template <class Archive>
    void serialize(Archive& archive, eeagl::vm::lang::command::RawCommand& command) {
        archive(command.op);
        eeagl::vm::lang::command::CommandStructure structure = eeagl::vm::lang::command::COMMAND_STRUCTURE.at(command.op);
        archiveOperand<Archive>(archive, structure.operand1, command.operand1);
        archiveOperand<Archive>(archive, structure.operand2, command.operand2);
        archiveOperand<Archive>(archive, structure.operand3, command.operand3);
    }

    template <class Archive>
    void serialize(Archive& archive, eeagl::vm::lang::command::Cell& cell) {
        archive(cell.commands);
    }

    template <class Archive>
    void serialize(Archive& archive, eeagl::vm::lang::Reference& ref) {
        archive(ref.direction, ref.reg);
    }

    template<class Archive>
    void serialize(Archive& archive, eeagl::vm::memory::MemoryDumpHeader& h) {
        archive(h.signature, h.version);
    }

    template<class Archive>
    void serialize(Archive& archive, eeagl::vm::memory::MemoryDump& dump) {
        archive(dump.header, dump.cells);
    }
}
#include "gtest/gtest.h"

#include "vm/lang/types.h"
#include "vm/lang/command/structure.h"

namespace eeagl::vm::lang::command {
    TEST(CommandStructureTest, OperandChecksumNumberEqual) {
        Operand operand;
        const int NUM = 33;
        operand.number = NUM;
        EXPECT_EQ(NUM, operandChecksum(Operator::Set, 1, operand));
    }

    TEST(CommandStructureTest, OperandChecksumNumberDiffer) {
        const int NUM = 33;

        Operand operand1;
        operand1.number = NUM;
        int checksum1 = operandChecksum(Operator::Set, 1, operand1);

        Operand operand2;
        operand2.number = NUM + 1;
        int checksum2 = operandChecksum(Operator::Set, 1, operand2);
        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_NE(checksum1, checksum2);
    }

    TEST(CommandStructureTest, OperandChecksumRegEqual) {
        const Register REG = Register::Register_1;

        Operand operand1;
        operand1.reg = REG;
        int checksum1 = operandChecksum(Operator::JumpIfEqualsReg, 0, operand1);

        Operand operand2;
        operand2.reg = REG;
        int checksum2 = operandChecksum(Operator::Increment, 0, operand2);
        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_EQ(checksum1, checksum2);
    }

    TEST(CommandStructureTest, OperandChecksumRegDiffer) {
        Operand operand1;
        operand1.reg = Register::Register_1;
        int checksum1 = operandChecksum(Operator::JumpIfEqualsReg, 0, operand1);

        Operand operand2;
        operand2.reg = Register::Register_2;
        int checksum2 = operandChecksum(Operator::Increment, 0, operand2);
        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_NE(checksum1, checksum2);
    }

    TEST(CommandStructureTest, OperandChecksumDirectionRegEqual) {
        Operand operand;
        const DirectionRegister DIR_REG = DirectionRegister::Directional_Register_1;
        operand.directionReg = DIR_REG;
        EXPECT_EQ(DIR_REG, operandChecksum(Operator::SetRandomDirection, 0, operand));
    }

    TEST(CommandStructureTest, OperandChecksumMinusOneOnError) {
        EXPECT_EQ(-1, operandChecksum(Operator::SetRandomDirection, 4, {}));
    }

    TEST(CommandStructureTest, OperandChecksumDirectionRegDiffer) {
        Operand operand1;
        operand1.directionReg = DirectionRegister::Directional_Register_1;
        int checksum1 = operandChecksum(Operator::SetRandomDirection, 0, operand1);

        Operand operand2;
        operand2.number = lang::MAX_CELL_INDEX;
        int checksum2 = operandChecksum(Operator::Set, 1, operand2);

        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_NE(checksum2, checksum1);
    }

    TEST(CommandStructureTest, OperandChecksumPointerEqual) {
        const CellCommandPointer POINTER = 1;
        Operand operand1;
        operand1.cellCommandPointer = POINTER;
        int checksum1 = operandChecksum(Operator::Jump, 0, operand1);

        Operand operand2;
        operand2.cellCommandPointer = POINTER;
        int checksum2 = operandChecksum(Operator::JumpIfEqualsRef, 2, operand2);

        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_EQ(checksum2, checksum1);
    }

    TEST(CommandStructureTest, OperandChecksumPointerDiffer) {
        Operand operand1;
        operand1.cellCommandPointer = 1;
        int checksum1 = operandChecksum(Operator::Jump, 0, operand1);

        Operand operand2;
        operand2.cellCommandPointer = 2;
        int checksum2 = operandChecksum(Operator::Jump, 0, operand2);

        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_NE(checksum2, checksum1);
    }

    TEST(CommandStructureTest, OperandChecksumReferenceEqual) {
        Reference ref = { Direction::Same, Register::Register_2 };

        Operand operand1;
        operand1.reference = ref;
        int checksum1 = operandChecksum(Operator::JumpIfEqualsRef, 0, operand1);

        Operand operand2;
        operand2.reference = ref;
        int checksum2 = operandChecksum(Operator::SwapIfEquals, 0, operand2);

        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_EQ(checksum2, checksum1);
    }

    TEST(CommandStructureTest, OperandChecksumReferenceDiffer) {

        Operand operand1;
        operand1.reference = { Direction::Same, Register::Register_2 };
        int checksum1 = operandChecksum(Operator::JumpIfEqualsRef, 0, operand1);

        Operand operand2;
        operand2.reference = { Direction::Up, Register::Register_1 };
        int checksum2 = operandChecksum(Operator::SwapIfEquals, 0, operand2);

        EXPECT_NE(-1, checksum1);
        EXPECT_NE(-1, checksum2);
        EXPECT_NE(checksum2, checksum1);
    }
}
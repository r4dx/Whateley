#include <iostream>
#include <memory>
#include <vector>

#include "memory_dump_test.h"
#include "vm/memory/memory.h"
#include "vm/lang/command.h"
#include "vm/memory/serialization.h"

#include <cereal/archives/portable_binary.hpp>

namespace eeagl::vm::memory {
    std::shared_ptr<MemoryDump> MemoryDumpTest::createSimpleMemoryDump() {
        auto cells = std::vector< std::vector <lang::Cell> >(1, std::vector <lang::Cell>(1, lang::Cell()));
        MemoryDump dump = { { MemoryDumpHeader::SIGNATURE, MemoryDumpHeader::CURRENT_VERSION }, cells };
        return std::make_shared<MemoryDump>(dump);
    }

    std::shared_ptr<std::istream> MemoryDumpTest::createUnderlyingStream(
        const std::string signature,
        int version,
        int xDimension,
        int yDimension) {

        std::vector < std::vector <lang::Cell> > cells(xDimension,
            std::vector<lang::Cell>(yDimension, lang::Cell()));

        MemoryDump dump = { { signature, version }, cells };
        std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
        {
            cereal::PortableBinaryOutputArchive oarchive(*ss);
            oarchive(dump);
        }
        return ss;
    }

    TEST_F(MemoryDumpTest, ReadSuccessful) {
        std::shared_ptr<std::istream> ss = createUnderlyingStream();
        ReadDumpResult result = MemoryDump::read(*ss);
        EXPECT_TRUE(result.succeed);
    }

    TEST_F(MemoryDumpTest, ReadInvalidSignature) {
        std::shared_ptr<std::istream> ss = createUnderlyingStream("AAAA");
        ReadDumpResult result = MemoryDump::read(*ss);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, ReadDumpResult::Error::INVALID_SIGNATURE);
    }

    TEST_F(MemoryDumpTest, ReadInvalidVersion) {
        std::shared_ptr<std::istream> ss = createUnderlyingStream(
            MemoryDumpHeader::SIGNATURE, MemoryDumpHeader::CURRENT_VERSION + 1);
        ReadDumpResult result = MemoryDump::read(*ss);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, ReadDumpResult::Error::INVALID_VERSION);
    }

    TEST_F(MemoryDumpTest, ReadError) {
        std::stringstream ss;
        ReadDumpResult result = MemoryDump::read(ss);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, ReadDumpResult::Error::READ_ERROR);
    }

    TEST_F(MemoryDumpTest, WriteError) {
        std::shared_ptr<MemoryDump> dump = createSimpleMemoryDump();

        std::stringstream ss(std::ios_base::in);
        ss.seekp(0, std::ios::end);
        WriteDumpResult result = dump->write(ss);
        EXPECT_FALSE(result.succeed);
        EXPECT_EQ(result.error, WriteDumpResult::Error::WRITE_ERROR);
    }

    TEST_F(MemoryDumpTest, WriteSucceed) {
        std::shared_ptr<MemoryDump> dump = createSimpleMemoryDump();

        lang::RawCommand testCommand;
        testCommand.op = lang::Operator::Increment;
        testCommand.operand1.reg = lang::Register::Register_2;
        dump->cells[0][0].commands[0] = testCommand;

        std::stringstream ss;
        WriteDumpResult writeResult = dump->write(ss);
        EXPECT_TRUE(writeResult.succeed);
        ss.seekg(0);
        ReadDumpResult readResult = MemoryDump::read(ss);
        EXPECT_TRUE(readResult.succeed);
        EXPECT_EQ(readResult.result->cells[0][0].commands[0].op, testCommand.op);
        EXPECT_EQ(readResult.result->cells[0][0].commands[0].operand1.reg, testCommand.operand1.reg);
    }
}
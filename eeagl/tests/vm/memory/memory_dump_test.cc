#include <iostream>
#include <memory>
#include <vector>

#include "memory_dump_test.h"
#include "vm/memory/memory.h"
#include "vm/lang/command.h"
#include "vm/memory/serialization.h"

#include <cereal/archives/portable_binary.hpp>

namespace eeagl {
    namespace vm {
        namespace memory {

            std::shared_ptr<MemoryDump> MemoryDumpTest::createSimpleMemoryDump() {
                std::stringstream ss;
                {
                    cereal::PortableBinaryOutputArchive oarchive(ss);
                    auto cells = std::vector< std::vector <lang::Cell> >(1, std::vector <lang::Cell>(1, lang::Cell()));
                    MemoryDump dump = { { MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION }, cells };                    
                    oarchive(dump);
                }
                ReadDumpResult result = MemoryDump::read(ss);
                return result.result;
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
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION + 1);
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

            TEST_F(MemoryDumpTest, WriteSucceed) {
                std::shared_ptr<std::istream> is = createUnderlyingStream(
                    MemoryDump::SIGNATURE, 
                    MemoryDump::CURRENT_VERSION, 
                    1, 
                    1);
                ReadDumpResult readResult = MemoryDump::read(*is);

                lang::RawCommand testCommand;
                testCommand.op = lang::Operator::Increment;
                testCommand.operand1.reg = lang::Register::Register_2;
                readResult.result->cells[0][0].commands[0] = testCommand;

                std::stringstream os;
                WriteDumpResult writeResult = readResult.result->write(os);
                EXPECT_TRUE(writeResult.succeed);
                os.seekp(0);
                ReadDumpResult readResult2 = MemoryDump::read(os);
                EXPECT_TRUE(readResult2.succeed);
                EXPECT_EQ(readResult2.result->cells[0][0].commands[0].op, testCommand.op);
                EXPECT_EQ(readResult2.result->cells[0][0].commands[0].operand1.reg, testCommand.operand1.reg);
            }
        }
    }
}
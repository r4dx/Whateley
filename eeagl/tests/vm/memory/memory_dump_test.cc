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
                    MemoryDumpHeader header;
                    header.signature = MemoryDump::SIGNATURE;
                    header.version = MemoryDump::CURRENT_VERSION;
                    header.xDimension = 1;
                    header.yDimension = 1;
                    oarchive(header);
                }
                lang::Cell cell;
                ss.write((char*)&cell, 1);
                ReadDumpResult result = MemoryDump::read(ss);
                return result.result;
            }

            std::shared_ptr<std::istream> MemoryDumpTest::createUnderlyingStream(
                const std::string signature,
                int version,
                int xDimension,
                int yDimension,
                int xRealSize,
                int yRealSize) {

                if (xRealSize < 0)
                    xRealSize = xDimension;

                if (yRealSize < 0)
                    yRealSize = yDimension;

                std::vector < std::vector <lang::Cell> > cells(xRealSize,
                    std::vector<lang::Cell>(yRealSize, lang::Cell()));

                MemoryDump dump = { { signature, version, xDimension, yDimension }, cells };
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
                EXPECT_TRUE(result.isSuccess);
            }

            TEST_F(MemoryDumpTest, ReadInvalidSignature) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream("AAAA");
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INVALID_SIGNATURE);
            }

            TEST_F(MemoryDumpTest, ReadInvalidVersion) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION + 1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INVALID_VERSION);
            }

            TEST_F(MemoryDumpTest, ReadXDimensionLessThanZero) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, -1, 1, 0, 0);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadYDimensionLessThanZero) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, 1, -1, 0, 0);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadXDimensionGreater) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, MemoryDump::MAX_DIMENSION + 1, 1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadYDimensionGreater) {
                std::shared_ptr<std::istream> ss = createUnderlyingStream(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, 1, MemoryDump::MAX_DIMENSION + 1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadError) {
                std::stringstream ss;
                ReadDumpResult result = MemoryDump::read(ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::READ_ERROR);
            }
        }
    }
}
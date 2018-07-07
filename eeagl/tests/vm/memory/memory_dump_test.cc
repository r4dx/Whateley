#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include "../../src/vm/memory/memory.h"
#include "../../src/vm/memory/memory_dump.h"
#include "../../src/vm/command.h"
#include <cereal/archives/portable_binary.hpp>

namespace eeagl {
    namespace vm {
        namespace memory {
            class MemoryDumpTest : public ::testing::Test {
            protected:
                virtual void SetUp() override {
                }

                virtual void TearDown() override {
                }

                std::shared_ptr<std::stringstream> createUnderlyingStreamForHeader(
                    const std::string signature = MemoryDump::SIGNATURE,
                    int version = MemoryDump::CURRENT_VERSION,
                    int xDimension = 1,
                    int yDimension = 1) {

                    MemoryDumpHeader header = { signature, version, xDimension, yDimension };
                    std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
                    {
                        cereal::PortableBinaryOutputArchive oarchive(*ss);
                        oarchive(header);
                    }

                    return ss;
                }

                std::shared_ptr<std::istream> createUnderlyingStream(
                    const std::string signature = MemoryDump::SIGNATURE,
                    int version = MemoryDump::CURRENT_VERSION,
                    int xDimension = 1,
                    int yDimension = 1) {

                    command::Cell cell;
                    std::shared_ptr<std::stringstream> ss = 
                        createUnderlyingStreamForHeader(signature, version, xDimension, yDimension);
                    (*ss).write((char*)&cell, xDimension * yDimension * sizeof(command::Cell));
                    return ss;
                }
            };

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
                std::shared_ptr<std::istream> ss = createUnderlyingStreamForHeader(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, -1, 1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadYDimensionLessThanZero) {
                std::shared_ptr<std::istream> ss = createUnderlyingStreamForHeader(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, 1, -1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadXDimensionGreater) {
                std::shared_ptr<std::istream> ss = createUnderlyingStreamForHeader(
                    MemoryDump::SIGNATURE, MemoryDump::CURRENT_VERSION, MemoryDump::MAX_DIMENSION + 1, 1);
                ReadDumpResult result = MemoryDump::read(*ss);
                EXPECT_FALSE(result.isSuccess);
                EXPECT_EQ(result.error, ReadDumpResult::Error::INCORRECT_DIMENSIONS);
            }

            TEST_F(MemoryDumpTest, ReadYDimensionGreater) {
                std::shared_ptr<std::istream> ss = createUnderlyingStreamForHeader(
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
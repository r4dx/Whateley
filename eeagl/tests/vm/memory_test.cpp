#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include "../../src/vm/memory.h"


namespace eeagle {
    namespace vm {
        namespace memory {
            class StreamDumpableMemoryTest : public ::testing::Test {
            private:

                // A lot of implementation details here
                void setupUnderlyingData() {
                    underlyingData = std::make_shared<std::stringstream>();
                    (*underlyingData).write("EAAGL", 5); // signature
                }
            protected:
                virtual void SetUp() override {
                    setupUnderlyingData();
                    CreateDumpableMemoryResult cr = StreamDumpableMemory::create(*underlyingData);
                    if (cr.succeed)
                        memory = cr.memory;
                }

                virtual void TearDown() override {
                }

                std::shared_ptr<std::stringstream> underlyingData;
                std::shared_ptr<StreamDumpableMemory> memory;
            };

            TEST(StreamDumpableMemoryTest, CreateInvalidSignature) {
                std::stringstream underlyingData;
                underlyingData.write("AAAAA", 5);
                CreateDumpableMemoryResult cr = StreamDumpableMemory::create(underlyingData);
                EXPECT_FALSE(cr.succeed);
                EXPECT_EQ(cr.error, CreateDumpableMemoryResult::Error::INVALID_SIGNATURE);
            }

            TEST(StreamDumpableMemoryTest, CreateCantRead) {
                std::stringstream underlyingData;
                underlyingData.write("A", 1);
                CreateDumpableMemoryResult cr = StreamDumpableMemory::create(underlyingData);
                EXPECT_FALSE(cr.succeed);
                EXPECT_EQ(cr.error, CreateDumpableMemoryResult::Error::READ_ERROR);
            }

            TEST_F(StreamDumpableMemoryTest, InvalidAddressOnNegativeGet) {
                CellAddress address(-1, -1);
                GetCellResult result = memory->get(address);
                EXPECT_EQ(GetCellResult::Error::INVALID_ADDRESS, result.error);
            }
        }
    }
}
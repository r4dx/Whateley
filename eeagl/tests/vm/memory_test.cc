#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include "../../src/vm/memory.h"
#include "../../src/vm/memory_dump.h"
#include "../../src/vm/command.h"
#include <cereal/archives/portable_binary.hpp>

namespace eeagle {
    namespace vm {
        namespace memory {
            class MemoryTest : public ::testing::Test {
            protected:
                virtual void SetUp() override {
                    dump = createSimpleMemoryDump();
                    memory = std::make_shared<Memory>(dump);
                }

                virtual void TearDown() override {
                }
                std::shared_ptr<MemoryDump> dump;
                std::shared_ptr<Memory> memory;

            private:
                std::shared_ptr<MemoryDump> createSimpleMemoryDump() {
                    std::stringstream ss;
                    {
                        cereal::PortableBinaryOutputArchive oarchive(ss);
                        MemoryDumpHeader header;
                        header.signature = "EEAGL";
                        header.version = 0;
                        header.xDimension = 1;
                        header.yDimension = 1;
                        oarchive(header);
                    }
                    command::Cell cell;
                    ss.write((char*)&cell, 1);
                    ReadDumpResult result = MemoryDump::read(ss);
                    return result.result;
                }
            };
            TEST_F(MemoryTest, InvalidAddressOnNegativeGet) {
                CellAddress address(-1, -1);
                GetCellResult result = memory->get(address);
                EXPECT_EQ(GetCellResult::Error::INVALID_ADDRESS, result.error);
            }
        }
    }
}
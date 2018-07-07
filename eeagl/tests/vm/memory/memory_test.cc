#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include "../../src/vm/memory/memory.h"
#include "../../src/vm/memory/memory_dump.h"
#include "../../src/vm/command.h"
#include <cereal/archives/portable_binary.hpp>
#include "memory_dump_test.h"

namespace eeagl {
    namespace vm {
        namespace memory {
            class MemoryTest : public ::testing::Test {
            protected:
                virtual void SetUp() override {
                    auto dump = MemoryDumpTest::createSimpleMemoryDump();
                    memory = std::make_shared<Memory>(dump);
                }

				std::shared_ptr<Memory> memory;
            };
            TEST_F(MemoryTest, InvalidAddressOnNegativeGet) {
                CellAddress address(-1, -1);
                GetCellResult result = memory->get(address);
                EXPECT_EQ(GetCellResult::Error::INVALID_ADDRESS, result.error);
            }
        }
    }
}
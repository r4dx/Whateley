#include <iostream>
#include "gtest/gtest.h"

#include "../../src/vm/memory.h"


namespace eeagle {
    namespace vm {
        namespace memory {

            class MemoryTest : public ::testing::Test {
            protected:
                virtual void SetUp() override {}

                virtual void TearDown() override {}
                Memory memory;
            };

            TEST_F(MemoryTest, InvalidAddressOnNegative) {
                CellAddress address(-1, -1);
                GetCellResult result = memory.get(address);
                EXPECT_EQ(GetCellResult::Error::INVALID_ADDRESS, result.error);
            }

        }
    }
}
#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <cstddef>

#include <cereal/archives/portable_binary.hpp>

#include "vm/memory/memory.h"
#include "vm/memory/memory_dump.h"
#include "vm/lang/command.h"
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

            TEST(MemoryAddressTest, toFlatIndex) {

                /*
                (0 1) (2 3) (4 5)
                (6 7) (8 9) (10 11)
                (12 13) (14 15) (16 17)
                (18 19) (20 21) (22 23)

                dimX = 3 dimY = 4 dimZ = 2

                how to get index:
                    15: (x=1, y=2, z=1)
                    15 = y*dim.z*dim.x + x*dim.z + z = 2*2*3 + 1*2 + 1
                    21: (1, 3, 1)
                    21 = 3*2*3 + 1*2 + 1

                how to get coords:
                    y = 15 / dimZ*dimX = 15 / 6 = 2
                    x = (15 - y*dim.z*dim.x ) / dimZ = 1
                    z = 15 - y*dim.z*dim.x + x*dim.z = 15 - 2*2*3 - 1*2 = 1
                */

                int dimX = 3;
                int dimZ = 2;

                MemoryAddress addr(1, 2, (std::byte)1);
                EXPECT_EQ(15, addr.toFlatIndex(dimX, dimZ));
                addr = { 0, 3, (std::byte)0 };
                EXPECT_EQ(18, addr.toFlatIndex(dimX, dimZ));
                addr = { 2, 3, (std::byte)0 };
                EXPECT_EQ(22, addr.toFlatIndex(dimX, dimZ));
            }

            TEST(MemoryAddressTest, fromFlatIndex) {
                int dimX = 3;
                int dimZ = 2;

                auto addr = MemoryAddress::fromFlatIndex(15, dimX, dimZ);
                EXPECT_EQ(MemoryAddress(1, 2, (std::byte)1), addr);
                
                addr = MemoryAddress::fromFlatIndex(18, dimX, dimZ);
                EXPECT_EQ(MemoryAddress(0, 3, (std::byte)0), addr);

                addr = MemoryAddress::fromFlatIndex(22, dimX, dimZ);
                EXPECT_EQ(MemoryAddress(2, 3, (std::byte)0), addr);
            }


            TEST_F(MemoryTest, InvalidAddressOnNegativeGet) {
                CellAddress address(-1, -1);
                GetCellResult result = memory->get(address);
                EXPECT_EQ(GetCellResult::Error::INVALID_ADDRESS, result.error);
            }
        }
    }
}
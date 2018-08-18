#include "gtest/gtest.h"
#include "vm/memory/memory.h"

namespace eeagl::vm::memory {
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
		int dimY = 1;
		int dimZ = 2;

		MemoryAddress addr(1, 2, (std::byte)1, dimX, dimY, dimZ);
		EXPECT_EQ(15, addr.toFlatIndex());
		addr = { 0, 3, (std::byte)0, dimX, dimY, dimZ };
		EXPECT_EQ(18, addr.toFlatIndex());
		addr = { 2, 3, (std::byte)0, dimX, dimY, dimZ };
		EXPECT_EQ(22, addr.toFlatIndex());
	}

	TEST(MemoryAddressTest, fromFlatIndex) {
		int dimX = 3;
		int dimY = 1;
		int dimZ = 2;

		auto addr = MemoryAddress::fromFlatIndex(15, dimX, dimY, dimZ);
		EXPECT_EQ(MemoryAddress(1, 2, (std::byte)1, dimX, dimY, dimZ), addr);

		addr = MemoryAddress::fromFlatIndex(18, dimX, dimY, dimZ);
		EXPECT_EQ(MemoryAddress(0, 3, (std::byte)0, dimX, dimY, dimZ), addr);

		addr = MemoryAddress::fromFlatIndex(22, dimX, dimY, dimZ);
		EXPECT_EQ(MemoryAddress(2, 3, (std::byte)0, dimX, dimY, dimZ), addr);
	}

	TEST(MemoryAddressTest, IncrementAddressSimple) {
		MemoryAddress address(0, 0, lang::toPointer(0), 1, 1, lang::CELL_SIZE);
		++address;
		EXPECT_EQ(address, MemoryAddress(0, 0, lang::toPointer(1), 1, 1, lang::CELL_SIZE));
	}

	TEST(MemoryAddressTest, IncrementAddressMoreThanCellSize) {
		int dimX = 2;
		int dimY = 1;
		int dimZ = lang::CELL_SIZE;
		MemoryAddress address(0, 0, lang::toPointer(lang::CELL_SIZE - 1), dimX, dimY, dimZ);
		++address;
		EXPECT_EQ(address, MemoryAddress(1, 0, lang::toPointer(0), dimX, dimY, dimZ));
	}

	TEST(MemoryAddressTest, IncrementAddressMoreThanCellSizeAndLastInARow) {
		int dimX = 2;
		int dimY = 2;
		int dimZ = lang::CELL_SIZE;

		MemoryAddress address(1, 0, lang::toPointer(lang::CELL_SIZE - 1), dimX, dimY, dimZ);
		++address;
		EXPECT_EQ(address, MemoryAddress(0, 1, lang::toPointer(0), dimX, dimY, dimZ));
	}

	TEST(MemoryAddressTest, IncrementAddressGoesBackWhenLastEntry) {
		int dimX = 2;
		int dimY = 2;
		int dimZ = lang::CELL_SIZE;

		MemoryAddress address(1, 1, lang::toPointer(lang::CELL_SIZE - 1), dimX, dimY, dimZ);
		++address;
		EXPECT_EQ(address, MemoryAddress(0, 0, lang::toPointer(0), dimX, dimY, dimZ));
	}
}
#pragma once
#include "gtest/gtest.h"
#include "../../src/vm/memory/memory_dump.h"
#include <sstream>
#include <string>

namespace eeagl {
	namespace vm {
		namespace memory {
			class MemoryDumpTest : public ::testing::Test {
			public:
				static std::shared_ptr<MemoryDump> createSimpleMemoryDump();
			protected:
				std::shared_ptr<std::stringstream> createUnderlyingStreamForHeader(
					const std::string signature = MemoryDump::SIGNATURE,
					int version = MemoryDump::CURRENT_VERSION,
					int xDimension = 1,
					int yDimension = 1);

				std::shared_ptr<std::istream> createUnderlyingStream(
					const std::string signature = MemoryDump::SIGNATURE,
					int version = MemoryDump::CURRENT_VERSION,
					int xDimension = 1,
					int yDimension = 1);
			};
		}
	}
}
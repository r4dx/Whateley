#include "memory_dump.h"
#include "serialization.h"

namespace eeagl::vm::memory {
    const std::string MemoryDumpHeader::SIGNATURE = "EEAGL";

	MemoryAddress MemoryDump::toAddress(int x, int y, lang::CellCommandPointer index) const {
		return MemoryAddress(x, y, index, cells[0].size(), cells.size(), lang::CELL_SIZE);
	}

    bool MemoryDump::isCompatible(MemoryAddress address) const {
        return (size_t)address.x < cells[0].size() && (size_t)address.y < cells.size() && address.index <= lang::MAX_CELL_INDEX;
    }

	ReadDumpResult MemoryDump::read(std::istream& is) {
        ReadDumpResult result;
        result.succeed = false;
        result.result = std::make_shared<MemoryDump>();

        try {
            cereal::PortableBinaryInputArchive iArchive(is);
            iArchive(*result.result);
        }
        catch (...) {
            result.error = ReadDumpResult::Error::READ_ERROR;
            return result;
        }

        if (result.result->header.signature != MemoryDumpHeader::SIGNATURE) {
            result.error = ReadDumpResult::Error::INVALID_SIGNATURE;
            return result;
        }
        if (result.result->header.version != MemoryDumpHeader::CURRENT_VERSION) {
            result.error = ReadDumpResult::Error::INVALID_VERSION;
            return result;
        }
        result.succeed = true;
        return result;
    }
    WriteDumpResult MemoryDump::write(std::ostream& os) {
        WriteDumpResult result;
        result.succeed = true;
        try {
            cereal::PortableBinaryOutputArchive oArchive(os);
            oArchive(*this);
        }
        catch (...) {
            result.succeed = false;
            result.error = WriteDumpResult::Error::WRITE_ERROR;
        }
        return result;
    }
}
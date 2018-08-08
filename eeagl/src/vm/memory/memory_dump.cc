#include "memory_dump.h"
#include "serialization.h"

namespace eeagl::vm::memory {
    const std::string MemoryDumpHeader::SIGNATURE = "EEAGL";

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
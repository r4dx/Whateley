#include "memory_dump.h"
#include "serialization.h"

namespace eeagl {
    namespace vm {
        namespace memory {
            const std::string MemoryDump::SIGNATURE = "EEAGL";

            ReadDumpResult MemoryDump::read(std::istream& is) {
                ReadDumpResult result;
                result.isSuccess = false;
                result.result = std::make_shared<MemoryDump>();

                try {
                    cereal::PortableBinaryInputArchive iArchive(is);
                    iArchive(*result.result);
                }
                catch (...) {
                    result.error = ReadDumpResult::Error::READ_ERROR;
                    return result;
                }

                if (result.result->header.signature != SIGNATURE) {
                    result.error = ReadDumpResult::Error::INVALID_SIGNATURE;
                    return result;
                }
                if (result.result->header.version != CURRENT_VERSION) {
                    result.error = ReadDumpResult::Error::INVALID_VERSION;
                    return result;
                }

                if (result.result->header.xDimension < 1 ||
                    result.result->header.yDimension < 1 ||
                    result.result->header.xDimension > MAX_DIMENSION ||
                    result.result->header.yDimension > MAX_DIMENSION) {

                    result.error = ReadDumpResult::Error::INCORRECT_DIMENSIONS;
                    return result;
                }
                result.isSuccess = true;
                return result;
            }
            WriteDumpResult MemoryDump::write(const MemoryDump& dump, std::ostream& os) {
                return WriteDumpResult();
            }
        }
    }
}
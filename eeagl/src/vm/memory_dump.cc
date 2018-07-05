#include "memory_dump.h"

namespace eeagle {
    namespace vm {
        namespace memory {
            const std::string MemoryDump::SIGNATURE = "EEAGL";

            MemoryDump::~MemoryDump() {
                delete[] cells;
            }

            ReadDumpResult MemoryDump::read(std::istream& is) {
                ReadDumpResult result;
                result.isSuccess = false;
                result.result = std::make_shared<MemoryDump>();

                try {
                    cereal::PortableBinaryInputArchive iArchive(is);
                    iArchive(result.result->header);
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


                long size = result.result->header.xDimension * result.result->header.yDimension;
                result.result->cells = new command::Cell[size];

                is.read((char*)result.result->cells, size * sizeof(command::Cell));
                result.isSuccess = true;
                return result;
            }
            WriteDumpResult MemoryDump::write(const MemoryDump& dump, std::ostream& os) {
                return WriteDumpResult();
            }
        }
    }
}
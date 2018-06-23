#include "memory.h"
#include <cstring>

namespace eeagle {
    namespace vm {
        namespace memory {

            SetMemoryResult Memory::set(MemoryAddress, command::RawCommand command) {
                SetMemoryResult result;
                result.error = SetMemoryResult::INVALID_ADDRESS;
                result.succeed = false;
                return result;
            }

            GetCellResult Memory::get(const CellAddress& address) const {
                GetCellResult result;
                result.succeed = false;
                result.error = GetCellResult::Error::INVALID_ADDRESS;
                return result;
            }

            StreamDumpableMemory::StreamDumpableMemory() { }

            bool checkSignature(std::istream& is, CreateDumpableMemoryResult& result) {
                const char SIGNATURE[] = "EEAGL";
                const int SIG_SIZE = sizeof(SIGNATURE) / sizeof(*SIGNATURE) - 1;
                char signature[SIG_SIZE + 1] = "";

                if (!is.read(signature, SIG_SIZE)) {
                    result.error = CreateDumpableMemoryResult::Error::READ_ERROR;
                    return false;
                }

                if (strcmp(signature, SIGNATURE) != 0) {
                    result.error = CreateDumpableMemoryResult::Error::INVALID_SIGNATURE;
                    return false;
                }
                return true;
            }

            CreateDumpableMemoryResult StreamDumpableMemory::create(std::istream& is) {
                CreateDumpableMemoryResult result;
                result.succeed = false;

                if (!checkSignature(is, result))
                    return result;

                result.succeed = true;
                return result;
            }

            void StreamDumpableMemory::save(std::ostream& os) {
            }
        }
    }
}
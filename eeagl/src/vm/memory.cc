#include "memory.h"
#include "memory_dump.h"
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

            Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) { }
        }
    }
}
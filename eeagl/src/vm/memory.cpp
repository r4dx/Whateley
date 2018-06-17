#include "memory.h"

namespace eeagle {
    namespace vm {
        namespace memory {
            GetCellResult Memory::get(const CellAddress& address) const {
                GetCellResult result;
                result.isSuccess = false;
                result.error = GetCellResult::Error::INVALID_ADDRESS;
                return result;
            }
        }
    }
}
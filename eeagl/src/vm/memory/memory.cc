#include "memory.h"
#include "memory_dump.h"
#include <cstring>

namespace eeagl::vm::memory {
    bool MemoryAddress::operator==(const MemoryAddress& rhs) const {
        return x == rhs.x && y == rhs.y && index == rhs.index;
    }
    bool MemoryAddress::operator!=(const MemoryAddress& rhs) const {
        return !operator==(rhs);
    }

    int MemoryAddress::toFlatIndex(int dimX, int dimZ) const {
        return y * dimZ * dimX + x * dimZ + (int)index;
    }

    MemoryAddress MemoryAddress::fromFlatIndex(int index, int dimX, int dimZ) {
        int y = index / (dimZ * dimX);
        int x = (index - y * dimZ * dimX) / dimZ;
        int z = index - y * dimZ * dimX - x * dimZ;

        return MemoryAddress( x, y, std::byte(z));
    }

    void Memory::incrementAddress(MemoryAddress& address) const {
        if ((int)address.index + 1 < lang::CELL_SIZE)
            address.index = lang::toPointer((int)address.index + 1);
        else {
            address.index = lang::toPointer(0);

            if (++address.x >= dump->cells[0].size()) {
                address.x = 0;
                if (++address.y >= dump->cells.size())
                    address.y = 0;
            }
        }
    }

    SetMemoryResult Memory::set(MemoryAddress, lang::RawCommand command) {
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

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
#include "memory.h"
#include "memory_dump.h"
#include <cstring>
#include <algorithm>

namespace eeagl::vm::memory {
	MemoryAddress Memory::toAddress(int x, int y, lang::CellCommandPointer index) const {
		return dump->toAddress(x, y, index);
	}

    DereferenceResult Memory::dereference(MemoryAddress address) const {
        if (!dump->isCompatible(address))
            return { false, Error::INVALID_ADDRESS };
        
        auto command = dump->cells[address.y][address.x].commands[(int)address.index];
        return { true, Error::NONE, command };
    }

    SwapResult Memory::swap(MemoryAddress address_1, MemoryAddress address_2) {
        if (!dump->isCompatible(address_1) || !dump->isCompatible(address_2))
            return { false, Error::INVALID_ADDRESS };

        auto tmp = dump->cells[address_1.y][address_1.x].commands[address_1.index];
        dump->cells[address_1.y][address_1.x].commands[address_1.index] = 
            dump->cells[address_2.y][address_2.x].commands[address_2.index];
        dump->cells[address_2.y][address_2.x].commands[address_2.index] = tmp;
        return { true };
    }

    template<typename T, size_t size>
    void swapBlocks(T (&arr1)[size], T (&arr2)[size], const int i1, const int i2) {
        auto lesserArray = (i1 >= i2 ? arr2 : arr1);
        auto biggerArray = (i1 >= i2 ? arr1 : arr2);
        int lesserIndex = (i1 >= i2 ? i2 : i1);
        int biggerIndex = (i1 >= i2 ? i1 : i2);

        for (int i = biggerIndex; i < size; i++) {
            std::swap(biggerArray[i], lesserArray[lesserIndex]);
            lesserIndex++;
        }
    }

    SwapResult Memory::swapCellBlocksTilEnd(MemoryAddress address_1, MemoryAddress address_2) {
        if (!dump->isCompatible(address_1) || !dump->isCompatible(address_2))
            return { false, Error::INVALID_ADDRESS };

        swapBlocks(
            dump->cells[address_1.y][address_1.x].commands, 
            dump->cells[address_2.y][address_2.x].commands, 
            address_1.index, 
            address_2.index);

        return { true };
    }

    Memory::Memory(std::shared_ptr<MemoryDump> dump) : dump(dump) {}
}
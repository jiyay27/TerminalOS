#pragma once
#include "IMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>

class FlatMemoryAllocator : public IMemoryAllocator
{
public:
    FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
        memory.resize(maximumSize, '.'); // Use resize instead of reserve
        initializeMemory();
    }

    ~FlatMemoryAllocator() {
        memory.clear();
    }

    void* allocate(size_t size) override {
        for (size_t i = 0; i < maximumSize - size + 1; i++) {
            if (!allocationMap[i] && canAllocateAt(i, size)) {
                allocateAt(i, size);
                return &memory[i];
            }
        }
        return nullptr;
    }

    void deallocate(void* ptr) override {
        size_t index = static_cast<char*>(ptr) - &memory[0];
        if (allocationMap[index]) {
            deallocateAt(index, 5); // Ensure the size is correct
        }
    }
        
    std::string visualizeMemory() override {
        return std::string(memory.begin(), memory.end());
    }

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    void initializeMemory() {
        std::fill(memory.begin(), memory.end(), '.');
        allocationMap.clear(); // Clear the map before use
    }

    bool canAllocateAt(size_t index, size_t size) const {
        for (size_t i = index; i < index + size; ++i) {
            if (allocationMap.find(i) != allocationMap.end() && allocationMap.at(i)) {
                return false;
            }
        }
        return (index + size <= maximumSize);
    }

    void allocateAt(size_t index, size_t size) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = true;
        }
        allocatedSize += size;
    }

    void deallocateAt(size_t index, size_t size) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = false;
        }
        allocatedSize -= size;
    }
};
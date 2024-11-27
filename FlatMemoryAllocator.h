#pragma once
#include "IMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <sstream>

// Singleton Memory Allocator Class
class FlatMemoryAllocator : public IMemoryAllocator {
public:
    static FlatMemoryAllocator& getInstance(size_t maximumSize = 1024);

    // Public Interface
    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    std::string visualizeMemory() override;

    // Delete copy and move constructors to enforce singleton behavior
    FlatMemoryAllocator(const FlatMemoryAllocator&) = delete;
    FlatMemoryAllocator& operator=(const FlatMemoryAllocator&) = delete;

    ~FlatMemoryAllocator();

private:
    // Private Constructor
    FlatMemoryAllocator(size_t maximumSize);

    // Private Methods
    void initializeMemory();
    bool canAllocateAt(size_t index, size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(size_t index, size_t size);
    size_t findAllocationSize(size_t index) const;
    void* createBackingStoreEntry(size_t size);
    std::string pointerToString(void* ptr) const;

    // Private Members
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory; // Memory representation
    std::vector<bool> allocationMap; // Allocation tracking
    std::unordered_map<void*, size_t> backingStoreAllocations; // Backing store tracking
    std::mutex allocatorMutex; // Ensures thread safety
};

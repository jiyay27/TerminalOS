#pragma once
#include "IMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <deque>
#include <sstream>
#include "Config.h"

// Singleton Memory Allocator Class
class FlatMemoryAllocator {
public:
    static FlatMemoryAllocator* getInstance();

    // Public Interface
    void* allocate(size_t size);
    void deallocate(void* ptr);
    std::string visualizeMemory();

    //Backing store checking
	bool isInBackingStore(void* ptr) const;
	void* backingToMain(void* ptr);
    void evictOldest();

    int computeMemoryUtil() const;
    int computeMemoryUsed() const;
    int computeMemoryAvail() const;

	size_t getMaximumSize() const;
    // Delete copy and move constructors to enforce singleton behavior
    FlatMemoryAllocator(const FlatMemoryAllocator&) = delete;
    FlatMemoryAllocator& operator=(const FlatMemoryAllocator&) = delete;

    ~FlatMemoryAllocator();

    static void initializeMemory();

private:
    // Private Constructor
    FlatMemoryAllocator();

    // Private Methods
    
    bool canAllocateAt(size_t index, size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(void* ptr, size_t size);
    size_t findAllocationSize(size_t index) const;
    void* createBackingStoreEntry(size_t size);
    std::string pointerToString(void* ptr) const;

    static FlatMemoryAllocator* instance;
    // Private Members
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory; // Memory representation
    std::vector<bool> allocationMap; // Allocation tracking
    std::unordered_map<void*, size_t> backingStoreAllocations; // Backing store tracking
    std::deque<void*> allocationOrder;
    std::mutex allocatorMutex; // Ensures thread safety
};

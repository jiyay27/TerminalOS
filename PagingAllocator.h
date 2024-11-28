#pragma once
#include "IMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include <deque>

// Singleton Memory Allocator Class
class PagingAllocator : public IMemoryAllocator {
public:
    static PagingAllocator& getInstance(size_t maximumSize = 1024, size_t frameSize = 256);

    // Public Interface
    std::vector<void*> aallocate(size_t size);
    void ddeallocate(std::vector<void*> ptr);

    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    std::string visualizeMemory() override;

    //Backing store checking
    bool isInBackingStore(void* ptr) const;
    void* backingToMain(void* ptr);

    // Delete copy and move constructors to enforce singleton behavior
    PagingAllocator(const PagingAllocator&) = delete;
    PagingAllocator& operator=(const PagingAllocator&) = delete;

    ~PagingAllocator();

	int getFrameSize() const { return frameSize; }

private:
    // Private Constructor
    PagingAllocator(size_t maximumSize, size_t frameSize);

    // Private Methods
    void initializeMemory();
    bool checkPageFit(size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(size_t index, size_t size);
    size_t findAllocationSize(size_t index) const;
    void* createBackingStoreEntry(size_t size);
    std::string pointerToString(void* ptr) const;

    // Private Members
	size_t maximumSize;                 // Maximum size of main memory
	size_t allocatedSize;               // Total allocated size of main memory
	size_t frameSize;                   // Size of each frame in main memory
    size_t numFrames;                   // Number of frames in main memory
	int numFreeFrames;				    // Number of free frames in main memory

    std::vector<char> memory;           // Memory representation
    std::vector<bool> allocationMap;    // Allocation tracking

    std::deque<int> freeFrameList;	    // List of free frames

    // Let P be the number of pages required by a process and M is the rolled value between 
    // min-mem-per-proc and max-mem-per-proc. P can be computed as M / mem-per-frame.

    std::unordered_map<void*, size_t> backingStoreAllocations; // Backing store tracking
    std::mutex allocatorMutex;          // Ensures thread safety
};
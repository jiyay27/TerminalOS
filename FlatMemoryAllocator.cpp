#include "FlatMemoryAllocator.h"

// Singleton Instance Getter
FlatMemoryAllocator& FlatMemoryAllocator::getInstance(size_t maximumSize) 
{
    static FlatMemoryAllocator instance(maximumSize);
    return instance;
}

// Constructor
FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
    : maximumSize(maximumSize), allocatedSize(0) {
    memory.resize(maximumSize, '.');
    allocationMap.resize(maximumSize, false);
    initializeMemory();
}

// Deconstructor
FlatMemoryAllocator::~FlatMemoryAllocator() {
    for (auto& [ptr, size] : backingStoreAllocations) {
        delete[] static_cast<char*>(ptr);
    }
    backingStoreAllocations.clear();
    memory.clear();
    allocationMap.clear();
}

// Initialize Memory
void FlatMemoryAllocator::initializeMemory() {
    std::fill(memory.begin(), memory.end(), '.');
    std::fill(allocationMap.begin(), allocationMap.end(), false);
}

// Allocate Memory
void* FlatMemoryAllocator::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(allocatorMutex);
    //debugger
	//std::cout << "Allocating memory of size " << size << std::endl;
    // Try to allocate in main memory
    for (size_t i = 0; i <= maximumSize - size + 1; i++) {
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            allocateAt(i, size);
            return &memory[i];
        }
    }

    // Allocate in backing store if main memory fails
    void* backingPtr = createBackingStoreEntry(size);
    if (backingPtr != nullptr) {
        return backingPtr;
    }
    return nullptr;
}

// Deallocate Memory
void FlatMemoryAllocator::deallocate(void* ptr) {
    //std::lock_guard<std::mutex> lock(allocatorMutex);

    if (ptr == nullptr) return;

    char* charPtr = static_cast<char*>(ptr);

    // Check if pointer belongs to main memory
    if (charPtr >= memory.data() && charPtr < memory.data() + memory.size()) {
        size_t index = charPtr - memory.data();
        if (index < maximumSize && allocationMap[index]) {
            size_t blockSize = findAllocationSize(index);
            deallocateAt(index, blockSize);
            //std::cout << "Deallocated memory in main memory at index " << index << " with size " << blockSize << std::endl;
        }
        else {
            std::cerr << "Attempted to deallocate an unallocated or invalid pointer in main memory." << std::endl;
        }
    }
    // Check if pointer belongs to backing store
    else if (backingStoreAllocations.find(ptr) != backingStoreAllocations.end()) {
        delete[] static_cast<char*>(ptr);
        backingStoreAllocations.erase(ptr);
        //std::cout << "Deallocated memory from backing store. Pointer: " << ptr << std::endl;
    }
    else {
        std::cerr << "Attempted to deallocate a pointer that is neither in main memory nor in backing store." << std::endl;
    }
}

// Visualize Memory
std::string FlatMemoryAllocator::visualizeMemory() {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    std::string visualization(memory.begin(), memory.end());
    visualization += "\nBacking Store Allocations: ";
    for (const auto& [ptr, size] : backingStoreAllocations) {
        visualization += "[" + pointerToString(ptr) + " - Size: " + std::to_string(size) + "] ";
    }
    return visualization;
}

// Check if pointer is in backing store
bool FlatMemoryAllocator::isInBackingStore(void* ptr) const {
	return backingStoreAllocations.find(ptr) != backingStoreAllocations.end();
}

// Try allocating memory from backing store to main memory
void* FlatMemoryAllocator::backingToMain(void* ptr) {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    if (ptr == nullptr) return nullptr;

    // Check if the pointer is in the backing store
    auto it = backingStoreAllocations.find(ptr);
    if (it != backingStoreAllocations.end()) {
        size_t size = it->second; // Retrieve the size from the map

        // Try to allocate in main memory
        for (size_t i = 0; i <= maximumSize - size; i++) {
            if (!allocationMap[i] && canAllocateAt(i, size)) {
                allocateAt(i, size);
                //std::cout << "Allocated memory from backing store to main memory. Pointer: " << ptr << std::endl;

                // Remove from the backing store map
                backingStoreAllocations.erase(it);
                return &memory[i];
            }
        }

        //std::cout << "Failed to allocate memory in main memory for pointer: " << ptr << std::endl;
    }

    return nullptr; // Return null if allocation fails or pointer is not in backing store
}

// Private: Check if allocation is possible
bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
    for (size_t i = index; i < index + size; ++i) {
        if (allocationMap[i]) {
            return false;
        }
    }
    return true;
}

// Private: Allocate memory at index
void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    for (size_t i = index; i < index + size; ++i) {
        allocationMap[i] = true;
        memory[i] = '#';
    }
    allocatedSize += size;
}

// Private: Deallocate memory at index
void FlatMemoryAllocator::deallocateAt(size_t index, size_t size) {
    for (size_t i = index; i < index + size; ++i) {
        allocationMap[i] = false;
        memory[i] = '.';
    }
    allocatedSize -= size;
}

// Private: Find allocation size
size_t FlatMemoryAllocator::findAllocationSize(size_t index) const {
    size_t size = 0;
    while (index + size < maximumSize && allocationMap[index + size]) {
        ++size;
    }
    return size;
}

// Private: Create backing store entry
void* FlatMemoryAllocator::createBackingStoreEntry(size_t size) {
    try {
        char* backingBlock = new char[size];
        std::fill(backingBlock, backingBlock + size, '.');
        backingStoreAllocations[backingBlock] = size;
        return static_cast<void*>(backingBlock);
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory in backing store: " << e.what() << std::endl;
        return nullptr;
    }
}

// Private: Convert pointer to string
std::string FlatMemoryAllocator::pointerToString(void* ptr) const {
    std::ostringstream oss;
    oss << ptr;
    return oss.str();
}

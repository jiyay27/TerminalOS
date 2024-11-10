#pragma once
#include "IMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>

class FlatMemoryAllocator : public IMemoryAllocator
{
public:
	FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
		memory.reserve(maximumSize);
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
			deallocateAt(index);
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
		std::fill(allocationMap.begin(), allocationMap.end(), false);
	}

	bool canAllocateAt(size_t index, size_t size) const {
		return (index + size <= maximumSize);
	}

	void allocateAt(size_t index, size_t size) {
		std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, true);
		allocatedSize += size;
	}

	void deallocateAt(size_t index) {
		allocationMap[index] = false;
	}
};
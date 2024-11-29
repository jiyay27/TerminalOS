#include "PagingAllocator.h"

PagingAllocator& PagingAllocator::getInstance(size_t maximumSize, size_t frameSize)
{
	static PagingAllocator instance(maximumSize, frameSize);
	instance.numFrames = maximumSize / frameSize;
	return instance;
}

PagingAllocator::PagingAllocator(size_t maximumSize, size_t frameSize)
{
	// Let P be the number of pages required by a process and M is the rolled value between 
	// min-mem-per-proc and max-mem-per-proc. P can be computed as M / mem-per-frame.


	this->maximumSize = maximumSize; // 1024 is 1KB -> 1024KB is 1MB
	this->frameSize = frameSize;
	this->allocatedSize = 0;
	memory.resize(maximumSize, '.');
	allocationMap.resize(maximumSize, false);
	initializeMemory();
}

PagingAllocator::~PagingAllocator()
{
	for (auto& [ptr, size] : backingStoreAllocations)
	{
		delete[] static_cast<char*>(ptr);
	}
	backingStoreAllocations.clear();
	memory.clear();
	allocationMap.clear();
}

void PagingAllocator::initializeMemory()
{
	std::fill(memory.begin(), memory.end(), '.');
	std::fill(allocationMap.begin(), allocationMap.end(), false);
	for (int i = 0; i < numFrames; i++)
	{
		freeFrameList.push_back(i);
	}
	this->numFreeFrames = this->numFrames;
}

std::vector<void*> PagingAllocator::allocate(size_t size)
{
	std::lock_guard<std::mutex> lock(allocatorMutex);

	size_t numFrames = (size + 1) / this->frameSize;

	std::vector<void*> allocatedPointers;
	std::vector<int> framesUsed;

	while (this->checkPageFit(size))
	{
		int freeFrame = this->freeFrameList.front();
		this->freeFrameList.pop_front();
		this->allocationMap[freeFrame] = true;
		framesUsed.push_back(freeFrame);
		memory[freeFrame] = '#';
		allocatedPointers.push_back(&memory[freeFrame]);
	}
	this->numFreeFrames -= framesUsed.size();
	this->allocatedSize += size;

	return allocatedPointers;

	// if return 0?
	/*if (!(this->checkPageFit(size)))
	{
		std::vector<int> framesUsed;
		for (int i = 0; i < numFrames; i++)
		{
			int freeFrame = this->freeFrameList.front();
			this->freeFrameList.pop_front();
			this->allocationMap[freeFrame] = true;
			framesUsed.push_back(freeFrame);
		}
		this->numFreeFrames -= framesUsed.size();
	}*/


	//// Allocate in backing store if main memory fails
	//void* backingPtr = createBackingStoreEntry(size);
	//if (backingPtr != nullptr) {
	//	return backingPtr;
	//}
	//return nullptr;
}

void PagingAllocator::deallocate(std::vector<void*> vecptr)
{
	if (vecptr[0] == nullptr) return;

	std::vector<int> framesUsed;

	for (int i = 0; i < memory.size(); i++)
	{
		for (int j = 0; j < vecptr.size(); i++)
		{
			if (&memory[i] == vecptr[j])
			{
				framesUsed.push_back(i);
			}
		}

	}

	for (int i = 0; i < vecptr.size(); i++)
	{
		allocationMap[framesUsed[i]] = false;
		memory[framesUsed[i]] = '.';
	}

	size_t size = this->frameSize * framesUsed.size();

	this->numFreeFrames += vecptr.size();
	this->allocatedSize += size;
}

std::string PagingAllocator::visualizeMemory()
{
	return "1";
}

bool PagingAllocator::checkPageFit(size_t size) const
{
	int freeFrames = size / this->frameSize;
	if (this->numFreeFrames >= freeFrames)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void PagingAllocator::allocateAt(size_t index, size_t size)
{

}

void PagingAllocator::deallocateAt(size_t index, size_t size)
{

}


size_t PagingAllocator::findAllocationSize(size_t index) const
{
	return 0;
}
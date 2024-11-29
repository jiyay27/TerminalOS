#include "PagingAllocator.h"

PagingAllocator* PagingAllocator::instance = nullptr;
PagingAllocator* PagingAllocator::getInstance()
{
	return instance;
}

void PagingAllocator::initializeMemory()
{
	instance = new PagingAllocator();
}

PagingAllocator::PagingAllocator()
{
	// Let P be the number of pages required by a process and M is the rolled value between 
	// min-mem-per-proc and max-mem-per-proc. P can be computed as M / mem-per-frame.
	Config config;
	config.setParamList("config.txt");

	this->maximumSize = config.getMaxMem(); // 1024 is 1KB -> 1024KB is 1MB
	this->frameSize = config.getMemFrame();
	this->allocatedSize = 0;

	this->numFrames = config.getMaxMem() / config.getMemFrame();
	this->numFreeFrames = this->numFrames;
	

	memory.resize(config.getMaxMem() , '.');
	allocationMap.resize(config.getMaxMem() , false);

	std::fill(memory.begin(), memory.end(), '.');
	std::fill(allocationMap.begin(), allocationMap.end(), false);

	for (int i = 0; i < this->numFrames; i++)
	{
		this->freeFrameList.push_back(i);
	}
	
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



std::vector<int> PagingAllocator::allocate(size_t size)
{
	//std::lock_guard<std::mutex> lock(allocatorMutex);

	size_t numFrames = (size + 1) / this->frameSize;

	std::vector<int> allocatedPointers;
	std::vector<int> framesUsed;

	//if (this->checkPageFit(size))
	//{
	//	for (size_t i = 0; i < numFrames; i++)
	//	{
	//		int freeFrame = this->freeFrameList.front();
	//		this->freeFrameList.pop_front();
	//		this->allocationMap[freeFrame] = true;
	//		framesUsed.push_back(freeFrame);
	//		memory[freeFrame] = '#';
	//		allocatedPointers.push_back(&memory[freeFrame]);
	//	}
	//	this->numFreeFrames -= framesUsed.size();
	//	this->allocatedSize += size;
	//	return allocatedPointers;
	//}

	if (this->checkPageFit(size))
	{
		for (size_t i = 0; i < numFrames; i++)
		{
			int freeFrame = this->freeFrameList.front();
			this->freeFrameList.pop_front();
			this->allocationMap[freeFrame] = true;
			framesUsed.push_back(freeFrame);
			memory[freeFrame] = '#';
			allocatedPointers.push_back(freeFrame);
			allocationOrder.push_back(freeFrame);
		}
		this->numFreeFrames -= framesUsed.size();
		this->allocatedSize += size;
		return allocatedPointers;
	}
	
	// if pages DO NOT fit
	if (!(this->checkPageFit(size)))
	{
		evictOldest();
	}
	else // eviction failed
	{
		allocatedPointers.push_back(NULL);
		return allocatedPointers;
	}
}

void PagingAllocator::deallocate(std::vector<int> vecptr)
{
	if (vecptr.empty())
	{
		//std::cerr << "Error: Deallocate failed. Vector is NULL." << std::endl;
		return;
	}

	std::vector<int> framesUsed = vecptr;

	for (int frameIndex : framesUsed) {
		this->freeFrameList.push_back(frameIndex);

		auto it = std::find(allocationOrder.begin(), allocationOrder.end(), frameIndex);
		if (it != allocationOrder.end()) {
			allocationOrder.erase(it);
		}
	}

	for (int i = 0; i < framesUsed.size(); i++)
	{
		allocationMap[framesUsed[i]] = false;
		memory[framesUsed[i]] = '.';
	}

	size_t size = this->frameSize * (size_t)framesUsed.size();

	this->numFreeFrames += vecptr.size();
	this->allocatedSize -= size;
}

std::string PagingAllocator::visualizeMemory()
{
	std::lock_guard<std::mutex> lock(allocatorMutex);

	std::string visualization(memory.begin(), memory.end());
	visualization += "\nBacking Store Allocations: ";
	for (const auto& [ptr, size] : backingStoreAllocations) {
		visualization += "[" + pointerToString(ptr) + " - Size: " + std::to_string(size) + "] ";
	}
	return visualization;
}

bool PagingAllocator::checkPageFit(size_t size) const
{
	int numFrames = size / this->frameSize;
	if (this->numFreeFrames >= numFrames)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PagingAllocator::isInBackingStore(std::vector<void*> vecptr) const
{
	return backingStoreAllocations.find(vecptr[0]) != backingStoreAllocations.end();
}

void* PagingAllocator::backingToMain(std::vector<void*> vecptr)
{
	std::lock_guard<std::mutex> lock(allocatorMutex);

	if (vecptr[0] == nullptr) return nullptr;

	
	

	return nullptr; // Return null if allocation fails or pointer is not in backing store
}

void PagingAllocator::evictOldest() {
	if (allocationOrder.empty()) {
		std::cerr << "Error: No frames to evict!" << std::endl;
		return;
	}

	int oldestFrame = allocationOrder.front(); // Get the oldest frame
	allocationOrder.pop_front();              // Remove it from tracking

	// Deallocate the oldest frame
	allocationMap[oldestFrame] = false;
	memory[oldestFrame] = '.';
	this->freeFrameList.push_back(oldestFrame);

	// Adjust counters
	this->numFreeFrames++;
	this->allocatedSize -= this->frameSize;
}

void* PagingAllocator::createBackingStoreEntry(size_t size) {
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
std::string PagingAllocator::pointerToString(void* ptr) const {
	std::ostringstream oss;
	oss << ptr;
	return oss.str();
}

int PagingAllocator::computeMemoryUtil() const
{
	int memoryUtil = this->allocatedSize * 100 / this->maximumSize;
	return memoryUtil;
}

int PagingAllocator::computeMemoryUsed() const
{
	return this->allocatedSize;
}

int PagingAllocator::computeMemoryAvail() const
{
	return this->maximumSize - this->allocatedSize;
}

size_t PagingAllocator::getMaximumSize() const {
	return maximumSize;
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
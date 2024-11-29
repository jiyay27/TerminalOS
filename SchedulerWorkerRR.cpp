#include "SchedulerWorkerRR.h"
#include "GlobalScheduler.h"
#include "IMemoryAllocator.h"
#include "Config.h"

SchedulerWorkerRR::SchedulerWorkerRR(int numCore)
{
	this->coreNum = numCore;
}


SchedulerWorkerRR::SchedulerWorkerRR(int numCore, int qq, int delay)
{
	this->coreNum = numCore;
	this->quantum = qq;
    this->delay = delay;
}

void SchedulerWorkerRR::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorkerRR::updateA()
{
	this->available = true;
}

void SchedulerWorkerRR::run() {
    Config config;
    config.setParamList("config.txt");

    size_t max = config.getMaxMem();
    size_t frame = config.getMemFrame();

    auto memoryAllocator = FlatMemoryAllocator::getInstance();
	auto pagingAllocator = PagingAllocator::getInstance();

    if (max == frame) // ! FLAT MEMORY ALLOCATOR
    {
        while (this->isRunning) {
            cpuClock++;
            this->updateA();

            if (this->process == nullptr && !this->processQueue.empty()) {
                this->process = this->processQueue.front();
            }

            if (this->process != nullptr) {
                // Memory allocation logic
                if (!this->process->getAllocationState()) {
                    size_t memoryRequired = this->process->getMemoryRequired();

                    if (memoryRequired > memoryAllocator->getInstance()->getMaximumSize()) {
                        std::cerr << "ERROR: Process " << this->process->getName()
                            << " requires more memory than available. Skipping.\n";
                        this->processQueue.pop(); // Remove process from queue
                        this->process = nullptr;
                        continue;
                    }

                    void* memory = memoryAllocator->getInstance()->allocate(memoryRequired);
                    if (memory) {
                        std::cout << "Memory allocated for process " << this->process->getName()
                            << " on core " << this->coreNum << std::endl;
                        this->process->setAssignedAt(memory);
                        this->process->setAllocationState(true);
                    }
                    else {
                        std::cerr << "ERROR: Memory allocation failed for process "
                            << this->process->getName() << ". Retrying...\n";
                        handleMemoryPressure(memoryAllocator->getInstance());
                        continue;
                    }
                }

                // Process execution logic
                if (this->process->isFinished()) {
                    finalizeProcess(memoryAllocator);
                }
                else {
                    executeProcess();
                }
            }
            else {
                // Scheduler is idle
				idleClock++;
                this->sleep(delay);
            }
        }
    }
    else // ! PAGING ALLOCATOR
    {
        while (this->isRunning)
        {
            cpuClock++;
            this->updateA();

            if (this->process == nullptr && !this->processQueue.empty()) {
                this->process = this->processQueue.front();
            }

            if (this->process != nullptr) {
                // Memory allocation logic
                if (!this->process->getAllocationState()) {
                    size_t memoryRequired = this->process->getMemoryRequired();

                    if (memoryRequired > pagingAllocator->getInstance()->getMaximumSize()) {
                        //std::cerr << "ERROR: Process " << this->process->getName()
                        //    << " requires more memory than available. Skipping.\n";
                        this->processQueue.pop(); // Remove process from queue
                        this->process = nullptr;
                        continue;
                    }

                    std::vector<int> memory = pagingAllocator->getInstance()->allocate(memoryRequired);
                    if (!memory.empty()) {
                        //std::cout << "\nMemory allocated for process " << this->process->getName()
                        //    << " on core " << this->coreNum << std::endl;
                        for (size_t i = 0; i < memory.size(); i++)
                        {
                            this->process->setAssignedAtVec(memory[i]);
                        }

                        this->process->setAllocationState(true);
                    }
                    else {
                        ////std::cerr << "ERROR: Memory allocation failed for process "
                        //    //<< this->process->getName() << ". Retrying...\n";
                        //handleMemoryPressurePaging(pagingAllocator->getInstance(), size);
                        continue;
                    }
                }

                // Process execution logic
                if (this->process->isFinished()) {
                    finalizeProcessPaging(pagingAllocator);
                }
                else {
                    executeProcess();
                }
            }
            else {
                // Scheduler is idle
                idleClock++;
                this->sleep(delay);
            }
        }
    }
}

// Handles memory pressure by evicting or sleeping
void SchedulerWorkerRR::handleMemoryPressure(FlatMemoryAllocator* flatMemoryAllocator)
{
    //std::cout << "Resolving memory pressure...\n";
    flatMemoryAllocator->getInstance()->evictOldest(); // Evict the oldest process if possible
    this->sleep(delay); // Avoid busy-waiting
}

// Finalize the process and release its resources
void SchedulerWorkerRR::finalizeProcess(FlatMemoryAllocator* flatMemoryAllocator)
{
    if (this->process->getAssignedAt()) {
        flatMemoryAllocator->getInstance()->deallocate(this->process->getAssignedAt());
        this->process->setAssignedAt(nullptr);
        this->process->setAllocationState(false);
    }

    this->process->setState(Process::FINISHED);
    this->processQueue.pop();
    this->process = nullptr;
    //std::cout << "Process completed and resources freed.\n";
}

// Handles memory pressure by evicting or sleeping
void SchedulerWorkerRR::handleMemoryPressurePaging(PagingAllocator* pagingAllocator, size_t size)
{
    //std::cout << "Resolving memory pressure...\n";
    pagingAllocator->getInstance()->evictOldest(); // Evict the oldest process if possible
    this->sleep(delay); // Avoid busy-waiting
}

// Finalize the process and release its resources
void SchedulerWorkerRR::finalizeProcessPaging(PagingAllocator* pagingAllocator)
{
    if (!this->process->getAssignedAtVec().empty()) {
        pagingAllocator->getInstance()->deallocate(this->process->getAssignedAtVec());
        this->process->setAssignedAtVec(NULL);
        this->process->setAllocationState(false);
    }

    this->process->setState(Process::FINISHED);
    this->processQueue.pop();
    this->process = nullptr;
    //std::cout << "Process completed and resources freed.\n";
}

// Execute process for the time quantum
void SchedulerWorkerRR::executeProcess() 
{
    for (int i = 0; i < this->quantum && !this->process->isFinished(); i++) {
        this->isOccupied();
        this->sleep(delay);
        this->process->executeInstruction();
    }

    if (!this->process->isFinished()) 
    {
        // Preempt unfinished process
        this->processQueue.push(this->process);
        this->processQueue.pop(); // Move it to the end of the queue
        this->process = this->processQueue.front();
    }
}

void SchedulerWorkerRR::addProcess(std::shared_ptr<Process> process)
{
	this->processQueue.push(process);
	this->process = processQueue.front();
}


void SchedulerWorkerRR::stop()
{
	this->isRunning = false;
}

bool SchedulerWorkerRR::isAvailable() const
{
	return this->available;
}

void SchedulerWorkerRR::isOccupied() {
	this->available = false;
}

bool SchedulerWorkerRR::processExists() const
{
	return this->process != nullptr;
}

int SchedulerWorkerRR::getCPUClock()
{
    std::lock_guard<std::mutex> lock(CPUmutex);
	return this->cpuClock;
}


int SchedulerWorkerRR::getIdleClock()
{
	std::lock_guard<std::mutex> lock(CPUmutex);
	return this->idleClock;
}
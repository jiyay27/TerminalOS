#include "SchedulerWorkerRR.h"
#include "GlobalScheduler.h"
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"

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
    auto& memoryAllocator = FlatMemoryAllocator::getInstance(100); // Singleton allocator
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

                if (memoryRequired > memoryAllocator.getMaximumSize()) {
                    std::cerr << "ERROR: Process " << this->process->getName()
                        << " requires more memory than available. Skipping.\n";
                    this->processQueue.pop(); // Remove process from queue
                    this->process = nullptr;
                    continue;
                }

                void* memory = memoryAllocator.allocate(memoryRequired);
                if (memory) {
                    std::cout << "Memory allocated for process " << this->process->getName()
                        << " on core " << this->coreNum << std::endl;
                    this->process->setAssignedAt(memory);
                    this->process->setAllocationState(true);
                }
                else {
                    std::cerr << "ERROR: Memory allocation failed for process "
                        << this->process->getName() << ". Retrying...\n";
                    handleMemoryPressure(memoryAllocator);
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
            this->sleep(delay);
        }
    }
}

// Handles memory pressure by evicting or sleeping
void SchedulerWorkerRR::handleMemoryPressure(FlatMemoryAllocator& memoryAllocator) {
    std::cout << "Resolving memory pressure...\n";
    memoryAllocator.evictOldest(); // Evict the oldest process if possible
    this->sleep(delay); // Avoid busy-waiting
}

// Finalize the process and release its resources
void SchedulerWorkerRR::finalizeProcess(FlatMemoryAllocator& memoryAllocator) {
    if (this->process->getAssignedAt()) {
        memoryAllocator.deallocate(this->process->getAssignedAt());
        this->process->setAssignedAt(nullptr);
        this->process->setAllocationState(false);
    }

    this->process->setState(Process::FINISHED);
    this->processQueue.pop();
    this->process = nullptr;
    std::cout << "Process completed and resources freed.\n";
}

// Execute process for the time quantum
void SchedulerWorkerRR::executeProcess() {
    for (int i = 0; i < this->quantum && !this->process->isFinished(); i++) {
        this->isOccupied();
        this->sleep(delay);
        this->process->executeInstruction();
    }

    if (!this->process->isFinished()) {
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
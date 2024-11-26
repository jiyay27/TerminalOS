#include "SchedulerWorker.h"
#include "GlobalScheduler.h"
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"

int maximumSize = 5000;

SchedulerWorker::SchedulerWorker(int numCore, int delay)
{
	this->coreNum = numCore;
	this->delay = delay;
	this->memoryAllocator = std::make_shared<FlatMemoryAllocator>(maximumSize);
}

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::updateA()
{
	this->available = true;
}

//implement memory allocation and deallocation
void SchedulerWorker::run()
{
	while (this->isRunning) 
	{
		cpuClock++;
		//std::lock_guard<std::mutex> lock(CPUWorkerMutex);
		/*if (!processExists()) {
			std::cout << "No process in queue. Worker is idle." << std::endl;
			stop();
			continue;
		}*/

		this->updateA();

		if (this->process != nullptr) 
		{
			void* memory = memoryAllocator->allocate(process->getMemoryRequired());

			if (memory != nullptr) {
				std::cout << "Allocated memory for process " << this->process->getName() << std::endl;
				std::cout << "Memory state: " << memoryAllocator->visualizeMemory() << std::endl;
				memoryAllocator->deallocate(memory);
			}
			else
			{
				std::cout << "Memory allocation failed for process " << this->process->getName() << std::endl;
				this->sleep(delay);
				continue;
			}

			if (this->process->isFinished()) 
			{
				this->updateA();
				this->process->setState(Process::FINISHED);
				this->processQueue.pop();
				if (!this->processQueue.empty())
				{
					this->process = this->processQueue.front();
				}
				else
				{
					this->stop();
				}
			}
			else
			{
				this->isOccupied();
				this->sleep(delay);
				this->process->executeInstruction();
			}
		}
	}
}

//adds the process into the queue
void SchedulerWorker::addProcess(std::shared_ptr<Process> process)
{
	//std::lock_guard<std::mutex> lock(CPUWorkerMutex);

	this->processQueue.push(process);
	this->process = processQueue.front();
}


void SchedulerWorker::stop()
{
	//std::lock_guard<std::mutex> lock(CPUWorkerMutex);

	this->isRunning = false;
}

bool SchedulerWorker::isAvailable() const
{
	return this->available;
}

void SchedulerWorker::isOccupied() {
	//std::lock_guard<std::mutex> lock(CPUWorkerMutex);

	this->available = false;
}

bool SchedulerWorker::processExists() const
{
	return this->process != nullptr;
}
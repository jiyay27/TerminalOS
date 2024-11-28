#include "SchedulerWorker.h"
#include "GlobalScheduler.h"
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"

int maximumSize = 100;

SchedulerWorker::SchedulerWorker(int numCore, int delay)
{
	this->coreNum = numCore;
	this->delay = delay;
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
	auto& memoryAllocator = FlatMemoryAllocator::getInstance(maximumSize);
	this->update(true);
	while (this->isRunning) 
	{
		cpuClock++;
		//std::lock_guard<std::mutex> lock(CPUWorkerMutex);
		this->updateA();

		if (this->process != nullptr) 
		{
			//check if the process is allocated in memory 
			//if not, allocate memory for the process and run the process
			//if the process is finished, deallocate the memory
			//else, run the process
			if (this->process->getAllocationState() == false)
			{
				//allocate memory for the process and set its internal memory address so that it can be accessed
				void* memory = memoryAllocator.allocate(this->process->getMemoryRequired());
				std::cout << "\nMemory allocated for process " << this->process->getName() << " on core: " << this->coreNum << std::endl;
				if (memory) {
					this->process->setAssignedAt(memory);
					this->process->setAllocationState(true);
				}
				else { std::cout<<"\nERROR: Cannot allocate memory." << std::endl; }
			}
			else 
			{
				if (this->process->isFinished() && this->process->getAssignedAt() != nullptr)
				{
					std::cout << "\nAddress: " << this->process->getAssignedAt() << std::endl;
					memoryAllocator.deallocate(this->process->getAssignedAt());
					this->process->setAllocationState(false);
					this->process->setAssignedAt(nullptr);	
					this->updateA();
					this->process->setState(Process::FINISHED);
					this->processQueue.pop();

					if (!this->processQueue.empty())
					{
						this->process = this->processQueue.front();
						//std::cout << "next process " << this->process->getName() << " on core: " << this->coreNum << std::endl;
					}
					else
					{
						std::cout << "\nProcess Queue is now empty. CPU ID: " << getCoreNum() << std::endl;
						std::cout << "root:\\>";
						//this->stop();
						this->process = nullptr;
					}
					
				}
				//when process not finished
				else 
				{
					//if not in backing store, then g!
					if (this->process->getAssignedAt() != nullptr && !memoryAllocator.isInBackingStore(this->process->getAssignedAt()))
					{
						this->isOccupied();
						this->sleep(delay);
						this->process->executeInstruction();
					}
					//if in backing store, try to assign to main mem
					else if (memoryAllocator.isInBackingStore(this->process->getAssignedAt()))
					{
						void* memory = memoryAllocator.backingToMain(this->process->getAssignedAt());
						if (memory)
						{
							this->process->setAssignedAt(memory);
							this->process->setAllocationState(true);
						}
						else { this->sleep(delay); 
						}
					}
				}	
			}
		}
		else if (!this->processQueue.empty())
		{
			this->process = this->processQueue.front();
		}
		else
		{
			this->sleep(delay);
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

int SchedulerWorker::getCoreNum() const
{
	return this->coreNum;
}
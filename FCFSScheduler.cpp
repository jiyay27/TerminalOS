#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{
    //this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
    processQueues[core].push(process);
}

// TODO: SEPARATE CHECKING OF CORE AVAILABILITY
void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core) 
{ 
	process->setCoreID(core);
	process->setState(Process::RUNNING);
	addProcess(process, core);
	assignProcess(process);
}

// assign process from queue to core
void FCFSScheduler::assignProcess(std::shared_ptr<Process> process)
{
	if (process != nullptr)
	{ 
		GlobalScheduler::getInstance()->getCPUWorker(process->getCPUCoreID())->addProcess(process);
		//GlobalScheduler::getInstance()->getCPUWorker(process->getCPUCoreID())->isOccupied();
	}
	else
	{
		std::cerr << "Process is null." << std::endl;
	}
}

//returns first core that is available
int FCFSScheduler::checkCores()
{
	for (int core = 0; core < numCores; core++)
	{
		if (GlobalScheduler::getInstance()->checkCoreAvailability(core) == true)
		{
			return core;
		}
	}
}

int FCFSScheduler::checkCoreQueue() {
	int index = -1;  // Default index when no core is available
	int minQueueSize = INT_MAX;  // Track the smallest queue size

	for (int i = 0; i < numCores; i++) {
		if (processQueues[i].empty()) {
			// Found an empty core, immediately return its index
			return i;
		}
		else if (processQueues[i].size() < minQueueSize) {
			// Update index and minQueueSize if a smaller queue is found
			minQueueSize = processQueues[i].size();
			index = i;
		}
	}
	return index;
}

String FCFSScheduler::getProcessfromQueue(int index) const
{
	return processQueues[index].front()->getName();
}

std::shared_ptr<Process> FCFSScheduler::getProcess(int core) const
{
	return this->processQueues[core].front();
}

void FCFSScheduler::execute() 
{
    for (int i = 0; i < numCores; i++)
    {
		if (GlobalScheduler::getInstance()->getCPUWorker(i)->processExists()) {
			GlobalScheduler::getInstance()->getCPUWorker(i)->update(true);
			//GlobalScheduler::getInstance()->getCPUWorker(i)->run();
		}
	}
}

//void FCFSScheduler::execute()
//{
//	for (int i = 0; i < numCores; i++)
//	{
//		if (!processQueues[i].empty())
//		{
//			if (getProcess(i)->isFinished())
//			{
//				GlobalScheduler::getInstance()->getCPUWorker(i)->updateA();
//				processQueues[i].pop();
//			}
//
//			FCFSScheduler::assignProcess(getProcess(i));
//			std::cout << "Process " << getProcess(i)->getName() << " assigned to core " << i << std::endl;
//		}
//	}
//
//	for (int i = 0; i < numCores; i++)
//	{
//		if (GlobalScheduler::getInstance()->getCPUWorker(i)->processExists()) 
//		{
//			GlobalScheduler::getInstance()->getCPUWorker(i)->update(true);
//			GlobalScheduler::getInstance()->getCPUWorker(i)->run();
//		}
//	}
//}

//void FCFSScheduler::execute() 
//{
//	//while (!allProcessesFinished()) 
//	//{
//		for (int core = 0; core < numCores; core++) 
//		{
//			if (!processQueues[core].empty()) 
//			{
//				std::shared_ptr<Process> process = getProcess(core);
//				
//				if (GlobalScheduler::getInstance()->getCPUWorker(core)->isAvailable()) 
//				{
//					assignProcess(process);
//				}
//
//				if (process->isFinished())
//				{
//					GlobalScheduler::getInstance()->getCPUWorker(core)->updateA();
//					GlobalScheduler::getInstance()->getCPUWorker(core)->run();
//					processQueues[core].erase(processQueues[core].begin());
//				}
//				else
//				{
//					GlobalScheduler::getInstance()->getCPUWorker(core)->addProcess(process);
//					GlobalScheduler::getInstance()->getCPUWorker(core)->isOccupied();
//
//					if (GlobalScheduler::getInstance()->getCPUWorker(core)->processExists())
//					{
//						GlobalScheduler::getInstance()->getCPUWorker(core)->update(true);
//						GlobalScheduler::getInstance()->getCPUWorker(core)->run();
//					}
//				}				
//			}
//		}
//	//}
//}


FCFSScheduler::FCFSScheduler()
{
    this->numCores = GlobalScheduler::getInstance()->getCoreCount();
	processQueues.resize(numCores);
    std::cout << "SIZE NG VECTOR " << processQueues.size() << std::endl;
}

FCFSScheduler::FCFSScheduler(int numCores)
{
    this->numCores = numCores;
	processQueues.resize(numCores);
}

void FCFSScheduler::printCores() {
	std::cout << "Number of cores: " << this->numCores << std::endl;
}

bool FCFSScheduler::allProcessesFinished() {
	for (const auto& queue : processQueues) {
		if (!queue.empty()) {
			return false;
		}
	}
	return true;
}

void FCFSScheduler::printProcessQueues() {
	for (size_t i = 0; i < processQueues.size(); ++i) 
	{
		std::cout << "Queue " << i << ":\n";

		// Create a copy of the current queue to print elements without modifying the original
		std::queue<std::shared_ptr<Process>> tempQueue = processQueues[i];

		if (tempQueue.empty()) {
			std::cout << "  [Empty]\n";
		}
		else {
			while (!tempQueue.empty()) {
				std::shared_ptr<Process> process = tempQueue.front();
				std::cout << "  " << process->getName() << "\n";
				tempQueue.pop();  // Remove the front element from the temporary queue
			}
		}
	}
}

void FCFSScheduler::stop()
{
	this->isRunning = false;
}
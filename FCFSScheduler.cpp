#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{
    //this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
    processQueues[core].push_back(process);
}

// TODO: SEPARATE CHECKING OF CORE AVAILABILITY
void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core) 
{ 
	addProcess(process, core);
	process->setCoreID(core);
	process->setState(Process::RUNNING);
}

// assign process from queue to core
void FCFSScheduler::assignProcess(std::shared_ptr<Process> process)
{

	GlobalScheduler::getInstance()->getCPUWorker(process->getCPUCoreID())->addProcess(process);
	GlobalScheduler::getInstance()->getCPUWorker(process->getCPUCoreID())->isOccupied();
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

int FCFSScheduler::checkCoreQueue()
{
	int index = -1;
	for (int i = 0; i < numCores; i++)
	{
		if (processQueues[i].empty())
		{
			std::cout << "PRINTING base case" << i << std::endl;
			index = i;
		}
		//Check size of each process queue 
		else if (processQueues[i].size() == processQueues[0].size() && i != 0)
		{
			std::cout << "PRINTING process(i) == process(0)" << i << std::endl;
			index = 0;
		}
		else if (processQueues[i].size() < processQueues[0].size() && i != 0)
		{
			std::cout << "PRINTING process() < process(0)" << i <<std::endl;
			index = i;
		}
	
	}
	return index;
}

//void FCFSScheduler::execute() 
//{
//    for (int i = 0; i < numCores; i++)
//    {
//		if (GlobalScheduler::getInstance()->getCPUWorker(i)->processExists()) {
//			GlobalScheduler::getInstance()->getCPUWorker(i)->update(true);
//			GlobalScheduler::getInstance()->getCPUWorker(i)->run();
//		}
//	}
//}

void FCFSScheduler::execute() {
	while (!allProcessesFinished()) {
		for (int core = 0; core < numCores; core++) {
			if (!processQueues[core].empty()) {
				auto process = processQueues[core].front();
				if (GlobalScheduler::getInstance()->getCPUWorker(core)->isAvailable()) {
					assignProcess(process);
					GlobalScheduler::getInstance()->getCPUWorker(core)->update(true);
				}
				GlobalScheduler::getInstance()->getCPUWorker(core)->run();
				if (process->isFinished()) {
					processQueues[core].erase(processQueues[core].begin());
				}
			}
		}
	}
}

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
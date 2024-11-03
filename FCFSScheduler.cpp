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
	process->setCoreID(core);
	process->setState(Process::RUNNING);
	addProcess(process, core);
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
	for (int i = 0; i < numCores; i++)
	{
		for (std::shared_ptr<Process> proc : processQueues[i])
		{
			if (proc->getState() == Process::FINISHED)
				{
					return i;
				}
		}
	}
}

void FCFSScheduler::execute() 
{
    for (int i = 0; i < numCores; i++)
    {
		if (GlobalScheduler::getInstance()->getCPUWorker(i)->processExists()) {
			GlobalScheduler::getInstance()->getCPUWorker(i)->update(true);
			GlobalScheduler::getInstance()->getCPUWorker(i)->run();
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
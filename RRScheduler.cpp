#include "RRScheduler.h"
#include "GlobalScheduler.h"
#include "ConsoleManager.h"
#include <algorithm>
#include <iomanip>

void RRScheduler::init()
{

}

RRScheduler::RRScheduler()
{
    this->numCores = GlobalScheduler::getInstance()->getCoreCount();
	this->timeQuantum = 20;
	processQueues.resize(numCores);
}

RRScheduler::RRScheduler(int qq)
{
    this->numCores = qq;
    this->timeQuantum = 20;
	processQueues.resize(numCores);
    processQueues.resize(numCores);
}

void RRScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
	processQueues[core].push_back(process);
}

void RRScheduler::assignCore(std::shared_ptr<Process> process, int core)
{
    process->setCoreID(core);
    process->setState(Process::RUNNING);
    addProcess(process, core);
    assignProcess(process);
}

void RRScheduler::assignProcess(std::shared_ptr<Process> process)
{
    if (process != nullptr)
    {
        GlobalScheduler::getInstance()->getCPUWorkerRR(process->getCPUCoreID())->addProcess(process);
        GlobalScheduler::getInstance()->getCPUWorkerRR(process->getCPUCoreID())->isOccupied();
    }
    else
    {
        std::cerr << "Process is null." << std::endl;
    }
}

int RRScheduler::checkCores()
{
	for (int core = 0; core < numCores; core++)
	{
		if (GlobalScheduler::getInstance()->checkCoreAvailability(core) == true)
		{
			return core;
		}
	}
	return -1; // No available core
}

int RRScheduler::checkCoreQueue() {
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

String RRScheduler::getProcessfromQueue(int index) const
{
    return processQueues[index].front()->getName();
}


void RRScheduler::execute() {
    for (int i = 0; i < numCores; i++)
    {
        if (GlobalScheduler::getInstance()->getCPUWorkerRR(i)->processExists()) {
            GlobalScheduler::getInstance()->getCPUWorkerRR(i)->update(true);
            //GlobalScheduler::getInstance()->getCPUWorker(i)->run();
        }
    }
}

void RRScheduler::run()
{
    while (this->schedulerRun == true)
    {
        srand(static_cast<unsigned int>(time(0)));
        std::ostringstream oss;
        oss << "proc-" << std::setw(2) << std::setfill('0') << this->i;
        std::string procName = oss.str();

        this->i++;
        ConsoleManager::getInstance()->createBaseScreen2(procName);
        int newCore = GlobalScheduler::getInstance()->getScheduler()->checkCoreQueue();
        GlobalScheduler::getInstance()->getScheduler()->assignCore(GlobalScheduler::getInstance()->getMostRecentProcess(), newCore);
        this->sleep(1000);
    }
}

void RRScheduler::schedulerStart()
{
    this->schedulerRun = true;
    this->start();
    this->sleep(10);
}

void RRScheduler::schedulerStop()
{
    this->schedulerRun = false;
}

bool RRScheduler::allProcessesFinished() {
    for (const auto& queue : processQueues) {
        if (!queue.empty()) {
            return false;
        }
    }
    return true;
}

void RRScheduler::printCores()
{
	for (int i = 0; i < numCores; i++)
	{
		std::cout << "Core " << i << ": ";
	}
}

void RRScheduler::printProcessQueues() {}

String RRScheduler::getName() const
{
    return this->name;
}
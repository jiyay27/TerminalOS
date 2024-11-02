#include "RRScheduler.h"
#include "GlobalScheduler.h"

void RRScheduler::init()
{
}
RRScheduler::RRScheduler()
{
	//this->timeQuantum = tq from config;
	processQueues.resize(numCores);
	currentIndex.resize(numCores, 0);
}
RRScheduler::RRScheduler(int numCores)
	: numCores(numCores){
    //this->timeQuantum = tq from config;
	processQueues.resize(numCores);
	currentIndex.resize(numCores, 0);
}

void RRScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
	processQueues[core].push_back(process);
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

void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core)
{
	process->setCoreID(core);
	process->setState(Process::RUNNING);
	GlobalScheduler::getInstance()->getCPUWorker(core)->addProcess(process);
	GlobalScheduler::getInstance()->getCPUWorker(core)->isOccupied();
	addProcess(process, core);
}

void RRScheduler::execute() {
    while (!allProcessesFinished()) {
        for (int core = 0; core < numCores; core++) {
            if (!processQueues[core].empty()) {
                int index = currentIndex[core];
                auto process = processQueues[core][index];

                // Execute the process for the time quantum
                for (int i = 0; i < timeQuantum && !process->isFinished(); i++) {
                    process->executeInstruction();
                }

                // Move to the next process in the queue
                currentIndex[core] = (index + 1) % processQueues[core].size();

                // If the process is finished, remove it from the queue
                if (process->isFinished()) {
                    processQueues[core].erase(processQueues[core].begin() + index);
                    // Adjust the current index if necessary
                    if (currentIndex[core] >= processQueues[core].size()) {
                        currentIndex[core] = 0;
                    }
                }
            }
        }
    }
}

bool RRScheduler::allProcessesFinished() {
    for (const auto& queue : processQueues) {
        if (!queue.empty()) {
            return false;
        }
    }
    return true;
}
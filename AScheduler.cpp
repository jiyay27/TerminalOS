#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName)
{
	this->schedulingAlgo = schedulingAlgo;
	this->pid = pid;
	this->processName = processName;
}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	for (auto process : this->processes)
	{
		if (process->getName() == processName)
		{
			return process;
		}
	}
	return nullptr;
}

void AScheduler::run()
{
	this->init();
	while (true)
	{
		this->execute();
	}
}

void AScheduler::stop()
{
	this->running = false;
}


AScheduler::AScheduler()
{
}
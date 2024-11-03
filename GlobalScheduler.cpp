// GlobalScheduler.cpp
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler* GlobalScheduler::getInstance()
{
	return sharedInstance;
}

void GlobalScheduler::initialize()
{
	sharedInstance = new GlobalScheduler();
}

void GlobalScheduler::destroy()
{
	delete sharedInstance;
}

void GlobalScheduler::selectScheduler(String algoName)
{
	if (algoName == "FCFS")
	{
		this->scheduler = std::make_shared<FCFSScheduler>(GlobalScheduler::getInstance()->getCoreCount());
	}
	else if (algoName == "RR")
	{
		this->scheduler = std::make_shared<RRScheduler>(GlobalScheduler::getInstance()->getCoreCount());
	}
	else
	{
		std::cerr << "Invalid algorithm name." << std::endl;
	}
}

std::shared_ptr<AScheduler> GlobalScheduler::getScheduler()
{
	return this->scheduler;
}

//DAPAT ANDITO APG RUN NG ALGORITHM
void GlobalScheduler::tick()
{
	if (this->scheduler) {
		this->scheduler->execute();
	} else {
		std::cerr << "Scheduler is not set." << std::endl;
	}
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process)
{
	if (process) {
		this->processList.push_back(process);
	}
	else {
		std::cerr << "Process is null." << std::endl;
	}
}

int GlobalScheduler::getProcessCount() const
{
	return this->processList.size();
}

std::shared_ptr<Process>& GlobalScheduler::getProcess(int index)
{
	if (index >= processList.size()) {
		throw std::out_of_range("Index out of range");
	}
	return this->processList[index];
}

//ASSIGNED NA YUNG CORE SA WORKER
GlobalScheduler::GlobalScheduler() // Initialize coreCount
{
	cpuWorkers.resize(coreCount);
	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<SchedulerWorker> worker = std::make_shared<SchedulerWorker>(i);
		this->cpuWorkers[i] = worker;
	}
}

//pag initialize ng console
//read config file, get core count, create workers, start workers
//dito palang naka bukas na ung threads
void GlobalScheduler::startThreads()
{
	if (this->scheduler) {
		this->scheduler->start();
	} else {
		std::cerr << "Scheduler is not set." << std::endl;
	}
	for (int i = 0; i < coreCount; i++)
	{
		this->cpuWorkers[i]->start();
	}
}

std::shared_ptr<Process> GlobalScheduler::getMostRecentProcess()
{
	return this->processList.back();
}

bool GlobalScheduler::checkCoreAvailability(int index) 
{
	if (this->cpuWorkers[index]->isAvailable()) {
		//std::cout << "Core is available." << std::endl;
		return true;
	}
	else
		return false;
}

GlobalScheduler::CPUWorkers& GlobalScheduler::getCPUWorkers()
{
	return this->cpuWorkers;
}

std::shared_ptr<SchedulerWorker> GlobalScheduler::getCPUWorker(int index)
{
	return this->cpuWorkers[index];
}

int GlobalScheduler::availableCores() {
	int count = 0;
	for (int i = 0; i < this->coreCount; i++)
	{
		if (this->cpuWorkers[i]->isAvailable())
		{
			count++;
		}
	}
	return count;
}

void GlobalScheduler::setCoreCount(int coreCount)
{
	this->coreCount = coreCount;
}

int GlobalScheduler::getCoreCount() const
{
	return this->coreCount;
}
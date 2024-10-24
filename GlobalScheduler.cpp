#include "GlobalScheduler.h"
#include "FCFSScheduler.h"

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
		std::shared_ptr<FCFSScheduler> fcfs = std::make_shared<FCFSScheduler>(4);
		this->scheduler = fcfs;

	}
	else if (algoName == "RR")
	{
		//RRScheduler* rr = new RRScheduler(1);
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
	this->scheduler->execute();
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->processList.push_back(process);
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
GlobalScheduler::GlobalScheduler()
{
	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<SchedulerWorker> worker = std::make_shared<SchedulerWorker>();
		this->cpuWorkers[i] = worker;
	}
	//scheduler
}

//pag initialize ng console
//read config file, get core count, create workers, start workers
//dito palang naka bukas na ung threads
void GlobalScheduler::startThreads()
{
	for (int i = 0; i < coreCount - 1; i++)
	{
		this->cpuWorkers[i]->start();
	}
}

std::shared_ptr<Process> GlobalScheduler::getMostRecentProcess()
{
	return processList.back();
}


int GlobalScheduler::checkCoreAvailability(int index) 
{
	if (this->cpuWorkers[index]->isAvailable()) {
		return index;
	}
	else
		return -1;
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
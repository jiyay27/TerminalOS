// GlobalScheduler.cpp
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "Config.h"
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
	if (algoName == "fcfs")
	{
		std::cout << "FCFS Scheduler selected." << std::endl;
		this->scheduler = std::make_shared<FCFSScheduler>(GlobalScheduler::getInstance()->getCoreCount());
	}
	else if (algoName == "rr")
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
	Config config;
	config.setParamList("config.txt");
	this->coreCount = config.getNumCPU();
	int qq = config.getQuantum();
	int delay = config.getDelays();
	cpuWorkers.resize(coreCount);
	cpuWorkersRR.resize(coreCount);
	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<SchedulerWorker> worker = std::make_shared<SchedulerWorker>(i, delay);
		this->cpuWorkers[i] = worker;
	}
	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<SchedulerWorkerRR> worker = std::make_shared<SchedulerWorkerRR>(i, qq, delay);
		this->cpuWorkersRR[i] = worker;
	}
}


void GlobalScheduler::startThreads()
{
	if (this->scheduler) {
		this->scheduler->start();
	} else {
		std::cerr << "Scheduler is not set." << std::endl;
	}
	if (this->scheduler->getName() == "FCFS")
	{
		for (int i = 0; i < coreCount; i++)
		{
			this->cpuWorkers[i]->start();
		}
	}
	else if (this->scheduler->getName() == "RR")
	{
		for (int i = 0; i < coreCount; i++)
		{
			this->cpuWorkersRR[i]->start();
		}
	}
	else
	{
		std::cerr << "Invalid algorithm name." << std::endl;
	}
}

std::shared_ptr<Process> GlobalScheduler::getMostRecentProcess()
{
	return this->processList.back();
}

bool GlobalScheduler::checkCoreAvailability(int index) 
{
	if (this->cpuWorkers[index]->isAvailable()) {
		return true;
	}
	else
		return false;
}


GlobalScheduler::CPUWorkers& GlobalScheduler::getCPUWorkers()
{
	return this->cpuWorkers;
}

GlobalScheduler::CPUWorkersRR& GlobalScheduler::getCPUWorkersRR()
{
	return this->cpuWorkersRR;
}

std::shared_ptr<SchedulerWorker> GlobalScheduler::getCPUWorker(int index)
{
	return this->cpuWorkers[index];
}

std::shared_ptr<SchedulerWorkerRR> GlobalScheduler::getCPUWorkerRR(int index)
{
	return this->cpuWorkersRR[index];
}

int GlobalScheduler::availableCores() 
{
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

int GlobalScheduler::availableCoresRR() 
{
	int count = 0;
	for (int i = 0; i < this->coreCount; i++)
	{
		if (this->cpuWorkersRR[i]->isAvailable())
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
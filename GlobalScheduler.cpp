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
	if (algoName == FCFS_SCHEDULER_NAME)
	{
		FCFSScheduler* fcfs = new FCFSScheduler(1);
		this->scheduler = fcfs;

	}
	else if (algoName == RR_SCHEDULER_NAME)
	{
		//RRScheduler* rr = new RRScheduler(1);
	}
	else
	{
		std::cerr << "Invalid algorithm name." << std::endl;
	}
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
	return processList[index];
}

//ASSIGNED NA YUNG CORE SA WORKER
GlobalScheduler::GlobalScheduler()
{
	for (int i = 0; i < coreCount; i++)
	{
		std::shared_ptr<SchedulerWorker> worker = std::make_shared<SchedulerWorker>();
		cpuWorkers[i] = worker;
	}
}
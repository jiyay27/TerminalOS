#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

SchedulerWorker::SchedulerWorker(int numCore)
{
	this->coreNum = numCore;
}

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (this->isRunning) {
		//GlobalScheduler::getInstance()->tick();
		this->process->executeInstruction();
	}
}

void SchedulerWorker::addProcess(int index)
{
	this->process = GlobalScheduler::getInstance()->getProcess(index);
}


void SchedulerWorker::stop()
{
	this->isRunning = false;
}

bool SchedulerWorker::isAvailable() const
{
	return this->available;
}

void SchedulerWorker::isOccupied() {
	this->available = false;
}
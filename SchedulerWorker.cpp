#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (this->isRunning) {
		GlobalScheduler::getInstance()->tick();
	}
}

void SchedulerWorker::addProcess(int index)
{
	this->process = GlobalScheduler::getInstance()->getProcess(index);
}


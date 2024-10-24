#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	// this->process->addCommand(print) replace with actual syntax
	while (this->isRunning) {
		GlobalScheduler::getInstance()->tick();
		// this->process->executeCurrentCommand()
	}

}

void SchedulerWorker::stop()
{
	this->isRunning = false;
}
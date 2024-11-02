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
		if (this->process != nullptr) {
			//GlobalScheduler::getInstance()->tick();
			this->process->executeInstruction();
			if (this->process->isFinished()) {
				this->available = true;
				stop();
				this->process = nullptr;
			}
		}
		else {
			//std::cerr << "IIIIIIIII" << std::endl;
		}
	}
}

void SchedulerWorker::addProcess(std::shared_ptr<Process> process)
{
	this->process = process;
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

bool SchedulerWorker::processExists() const
{
	return this->process != nullptr;
}
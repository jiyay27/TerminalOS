#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

SchedulerWorker::SchedulerWorker(int numCore,int delay)
{
	this->coreNum = numCore;
	this->delay = delay;
}

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::updateA()
{
	this->available = true;
}

void SchedulerWorker::run()
{
	while (this->isRunning) 
	{
		this->updateA();
		if (this->process != nullptr) 
		{
			if (this->process->isFinished()) 
			{
				this->updateA();
				this->process->setState(Process::FINISHED);
				this->processQueue.pop();
				if (!this->processQueue.empty())
				{
					this->process = this->processQueue.front();
				}
				else
				{
					this->stop();
				}
			}
			else
			{
				this->isOccupied();
				this->sleep(delay);
				this->process->executeInstruction();
			}
		}
	}
}

void SchedulerWorker::addProcess(std::shared_ptr<Process> process)
{
	//this->process = process;
	this->processQueue.push(process);
	this->process = processQueue.front();
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
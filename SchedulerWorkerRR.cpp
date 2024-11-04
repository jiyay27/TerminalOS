#include "SchedulerWorkerRR.h"
#include "GlobalScheduler.h"

SchedulerWorkerRR::SchedulerWorkerRR(int numCore)
{
	this->coreNum = numCore;
}


SchedulerWorkerRR::SchedulerWorkerRR(int numCore, int qq)
{
	this->coreNum = numCore;
	this->quantum = qq;
}

void SchedulerWorkerRR::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorkerRR::updateA()
{
	this->available = true;
}

void SchedulerWorkerRR::run()
{
    while (this->isRunning)
    {
        this->updateA();
        if (this->process != nullptr)
        {
            if (this->process->isFinished())
            {
                this->available = true;
                this->process->setState(Process::FINISHED);
                this->processQueue.pop();

                if (!this->processQueue.empty())
                {
                    // Sort the remaining processes in the queue based on burst time
                    std::vector<std::shared_ptr<Process>> tempProcesses;
                    while (!this->processQueue.empty())
                    {
                        tempProcesses.push_back(this->processQueue.front());
                        this->processQueue.pop();
                    }

                    // Sort the vector based on the burst time (command list count)
                    std::sort(tempProcesses.begin(), tempProcesses.end(),
                        [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
                            return a->getCommandListCount() < b->getCommandListCount();
                        });

                    // Push sorted processes back to the queue
                    for (const auto& proc : tempProcesses)
                    {
                        this->processQueue.push(proc);
                    }

                    // Set the new process
                    this->process = this->processQueue.front();
                }
                else
                {
                    this->stop();
                }
            }
            else
            {
                // Execute for the duration of the quantum
                for (int i = 0; i < this->quantum && !this->process->isFinished(); i++)
                {
                    this->isOccupied();
                    this->process->executeInstruction();
                }

                // Preempt the current process if it hasn't finished
                if (!this->process->isFinished())
                {
                    std::shared_ptr<Process> front = this->processQueue.front();
                    this->processQueue.pop();
                    this->processQueue.push(front); // Reinsert the current process to the end of the queue
                }
                this->process = this->processQueue.front(); // Get the next process
            }
        }
    }
}

void SchedulerWorkerRR::addProcess(std::shared_ptr<Process> process)
{
	//this->process = process;
	this->processQueue.push(process);
	this->process = processQueue.front();
}


void SchedulerWorkerRR::stop()
{
	this->isRunning = false;
}

bool SchedulerWorkerRR::isAvailable() const
{
	return this->available;
}

void SchedulerWorkerRR::isOccupied() {
	this->available = false;
}

bool SchedulerWorkerRR::processExists() const
{
	return this->process != nullptr;
}
#pragma once

#include <iostream>
#include <vector>
#include "Process.h"
#include "AScheduler.h"

class RRScheduler : public AScheduler
{
public:
	RRScheduler();
	RRScheduler(int cores);
	~RRScheduler() = default;
	bool allProcessesFinished();
	void init() override;
	void execute() override;

	void addProcess(std::shared_ptr<Process>, int core) override; // add process to core worker
	void assignCore(std::shared_ptr<Process> process, int core) override; // assign core to process
	int checkCores() override; // check if core is available
	int checkCoreQueue() override;
	String getProcessfromQueue(int index) const override;
	void printCores() override;
private:
	int numCores;
	int timeQuantum;
	std::vector<std::vector<std::shared_ptr<Process>>> processQueues;
	std::vector<int> currentIndex;
};

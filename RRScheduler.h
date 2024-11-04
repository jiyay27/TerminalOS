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

	void run() override;
	void init() override;
	void execute() override;

	void addProcess(std::shared_ptr<Process>, int core) override; // add process to core worker
	void assignCore(std::shared_ptr<Process> process, int core) override; // assign core to process
	int checkCores() override; // check if core is available
	int checkCoreQueue() override;
	void assignProcess(std::shared_ptr<Process> process);
	String getProcessfromQueue(int index) const override;
	void printCores() override;
	void printProcessQueues();

	void schedulerStart();
	void schedulerStop();

	String getName() const override;

private:
	String name = "RR";
	int numCores;
	int timeQuantum = 100;

	bool schedulerRun = false;
	int i = 1;

	std::vector<std::vector<std::shared_ptr<Process>>> processQueues;
	std::vector<int> currentIndex;
};

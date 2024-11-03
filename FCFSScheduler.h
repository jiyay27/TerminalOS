#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "Process.h"
#include "AScheduler.h"

class FCFSScheduler : public AScheduler
{
public:
    FCFSScheduler();
    FCFSScheduler(int cores);
    ~FCFSScheduler() = default;

    void init() override;
    void execute() override;

    void addProcess(std::shared_ptr<Process>, int core) override; // add process to core worker
	void assignCore(std::shared_ptr<Process> process, int core) override; // assign core to process
    void assignProcess(std::shared_ptr<Process> process);
    int checkCores() override; // check if core is available
    int checkCoreQueue() override;
	std::shared_ptr<Process> getProcess(int core) const;
	void printCores() override;
    String getProcessfromQueue(int index) const override;
	bool allProcessesFinished();
    void printProcessQueues();
    void stop();
private:
    int isRunning = true;
    int numCores;
    std::vector<std::queue<std::shared_ptr<Process>>> processQueues;
    std::vector<int> currentIndex;
};

#pragma once

#include <iostream>
#include <vector>
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
	int checkCores() override; // check if core is available
	void printCores() override;
private:
    int cores;
    int numCores;
    std::vector<std::vector<std::shared_ptr<Process>>> processQueues;
};

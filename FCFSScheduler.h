#pragma once

#include <iostream>
#include <vector>
#include "Process.h"
#include "AScheduler.h"

class FCFSScheduler : public AScheduler
{
public:
    //FCFSScheduler() { int numcores = 4; };
    FCFSScheduler();// : numCores(cores), processQueues(cores) {};
    ~FCFSScheduler() = default;

    void init() override;
    void execute() override;

    void addProcess(std::shared_ptr<Process>, int core) override; // add process to core worker
    int assignCore(std::shared_ptr<Process> process) override;
	void printCores() override;
private:
    int numCores;
    //std::vector<std::vector<std::shared_ptr<Process>>> processQueues;
};

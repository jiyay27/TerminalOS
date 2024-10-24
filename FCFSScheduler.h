#pragma once

#include <iostream>
#include <vector>
#include "Process.h"
#include "AScheduler.h"

class FCFSScheduler : public AScheduler
{
public:
    FCFSScheduler(int cores) : numCores(cores), processQueues(cores) {};

    void init() override;
    void addProcess(const Process& process, int core); // add process to core worker
    void execute() override;
    void assignCore(std::shared_ptr<Process> process);
private:
    int numCores;
    std::vector<std::vector<Process>> processQueues;
};

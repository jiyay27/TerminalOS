#pragma once

#include <iostream>
#include <vector>
#include "Process.h"
#include "AScheduler.h"

class FCFSScheduler : public AScheduler
{
public:
    FCFSScheduler(int cores) : numCores(cores), processQueues(cores) {};

    void initialize() const;
    void addProcess(Process& process, int core) const; // add process to core worker
    void sortProcessQueues() const;
    void runScheduler() const;

private:
    int numCores;
    std::vector<std::vector<Process>> processQueues;
};

#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include "Process.h"
#include "AScheduler.h"
#include "SchedulerWorker.h"

class FCFSScheduler : public AScheduler
{
public:
    typedef std::vector<std::shared_ptr<SchedulerWorker>> CPUWorkers;
    FCFSScheduler();
    FCFSScheduler(int cores);
    ~FCFSScheduler() = default;

    void run() override;
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

    virtual void schedulerStart();
    virtual void schedulerStop();

    String getName() const override;
private:
	String name = "FCFS";
    int isRunning = true;
    int numCores;
    
	std::mutex schedulerMutex;

    int batch;
    bool schedulerRun;
    int i = 1;

	CPUWorkers cpuWorkers;
    std::vector<std::queue<std::shared_ptr<Process>>> processQueues;
    std::vector<int> currentIndex;
};

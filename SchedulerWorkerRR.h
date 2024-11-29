#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include "Process.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

class SchedulerWorkerRR : public IETThread
{
public:
	SchedulerWorkerRR(int coreNum);
	SchedulerWorkerRR(int coreNum, int qq, int delay);
	~SchedulerWorkerRR() = default;

	void update(bool isRunning);
	void updateA();
	void run() override;
	void addProcess(std::shared_ptr<Process> process);
	bool isAvailable() const;
	void stop();
	void isOccupied();
	bool processExists() const;

	int getCPUClock();
	int getIdleClock();
private:
	bool isRunning = true; 
	bool available = true;
	int coreNum;
	int quantum;
	int delay;
	int cpuClock = 0;
	int idleClock = 0;
	std::shared_ptr<Process> process;
	std::queue<std::shared_ptr<Process>> processQueue;
	std::mutex CPUmutex;
	void handleMemoryPressure(FlatMemoryAllocator* pagingAllocator);
	void finalizeProcess(FlatMemoryAllocator* pagingAllocator);

	void handleMemoryPressurePaging(PagingAllocator* pagingAllocator);
	void finalizeProcessPaging(PagingAllocator* pagingAllocator);
	
	void executeProcess();
};


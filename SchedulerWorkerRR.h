#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include "Process.h"
#include "FlatMemoryAllocator.h"

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
private:
	bool isRunning = true;
	bool available = true;
	int coreNum;
	int quantum;
	int delay;
	int cpuClock = 0;
	std::shared_ptr<Process> process;
	std::queue<std::shared_ptr<Process>> processQueue;
	

	void handleMemoryPressure(FlatMemoryAllocator& memoryAllocator);
	void finalizeProcess(FlatMemoryAllocator& memoryAllocator);
	void executeProcess();
};


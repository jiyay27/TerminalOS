#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include "Process.h"

class SchedulerWorkerRR : public IETThread
{
public:
	SchedulerWorkerRR(int coreNum);
	SchedulerWorkerRR(int coreNum, int qq);
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
	int quantum = 100;
	std::shared_ptr<Process> process;
	std::queue<std::shared_ptr<Process>> processQueue;
};


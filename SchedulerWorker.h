#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include "Process.h"

class SchedulerWorker : public IETThread
{
public:
	SchedulerWorker(int coreNum, int delay);
	~SchedulerWorker() = default;

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
	int delay;
	std::shared_ptr<Process> process;
	std::queue<std::shared_ptr<Process>> processQueue;
};


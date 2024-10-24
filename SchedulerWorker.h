#pragma once
#include <iostream>
#include <string>
#include <thread>
#include "Process.h"

class SchedulerWorker : public IETThread
{
public:
	void update(bool isRunning);
	void run() override;
	void addProcess(int index);
	bool isAvailable() const;
	void stop();
	void isOccupied();
private:
	bool isRunning = false;
	bool available = true; //kung gamit na ung cpu
	std::shared_ptr<Process> process;
};


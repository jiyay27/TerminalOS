#pragma once
#include <iostream>
#include <string>
#include <thread>

class SchedulerWorker : public IETThread
{
public:
	void update(bool isRunning);
	void run() override;
	void addProcess(int index);
	void stop();

private:
	bool isRunning;
	bool available; //kung gamit na ung cpu
	std::shared_ptr<Process> process;
};


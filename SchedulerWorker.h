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
private:
	bool isRunning;
	std::shared_ptr<Process> process;
};


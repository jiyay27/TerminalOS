#pragma once
#include <iostream>
#include <string>
#include <thread>

class SchedulerWorker : public IETThread
{
public:
	void update(bool isRunning);
	void run() override;

private:
	bool isRunning;
};


#pragma once
#include <iostream>
#include <string>
#include <thread>

class SchedulerWorker
{
public:
	void update(bool isRunning);
	void run();

private:
	bool isRunning;
};
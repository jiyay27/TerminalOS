#pragma once
#include "AScheduler.h"

class GlobalScheduler
{
public:
	enum SchedulingAlgorithm
	{
		FCFS,
		RR
	};

	GlobalScheduler();
	~GlobalScheduler() = default;

	void init();
	void execute();

private:
	SchedulingAlgorithm schedulingAlgo;
	int pid;
	String processName;
	std::vector<std::shared_ptr<Process>> processes;
	bool running = true;
};
#pragma once
#include <sstream>
#include <unordered_map>

#include "IETThread.h"
#include "Process.h"
#include "TypedefRepo.h"

static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const String RR_SCHEDULER_NAME = "RRScheduler";


class AScheduler : public IETThread
{
public:
	AScheduler();
	enum SchedulingAlgorithm
	{
		FCFS, 
		RR
	};

	AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName);

	void addProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> findProcess(String processName);
	void run() override;
	void stop();

	virtual void init() = 0;
	virtual void execute() = 0;

private:
	SchedulingAlgorithm schedulingAlgo;
	int pid;
	String processName;
	std::vector<std::shared_ptr<Process>> processes;
	bool running = true;
};
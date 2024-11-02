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
	enum SchedulingAlgorithm
	{
		FCFS, 
		RR
	};

	AScheduler();
	AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName);
	~AScheduler() = default;

	std::shared_ptr<Process> findProcess(String processName);
	void run() override;
	void stop();

	virtual int checkCores() = 0;
	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void addProcess(std::shared_ptr<Process> process, int core) = 0;
	virtual void assignCore(std::shared_ptr<Process> process, int core) = 0;
	virtual void printCores() = 0;

	friend class GlobalScheduler;
private:
	SchedulingAlgorithm schedulingAlgo;
	int pid;
	String processName;
	bool running = true;
protected:
	std::vector<std::shared_ptr<Process>> processes;
};
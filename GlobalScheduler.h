#pragma once
#include "AScheduler.h"
#include "SchedulerWorker.h"
class GlobalScheduler  
{
public:

	typedef std::unordered_map<int, std::shared_ptr<SchedulerWorker>> CPUWorkers;

	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();

	void tick();

	void addProcess(std::shared_ptr<Process> process);
	int getProcessCount() const;
	std::shared_ptr<Process>& getProcess(int index);
	void startThreads();
	void selectScheduler(String algoName);


private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	AScheduler* scheduler;
	CPUWorkers cpuWorkers;

	int coreCount;

	std::vector<std::shared_ptr<Process>> processList;
};
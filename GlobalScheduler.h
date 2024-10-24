#pragma once
#include "AScheduler.h"
#include "SchedulerWorker.h"
#include "FCFSScheduler.h"
class GlobalScheduler  
{
public:
	typedef std::vector<std::shared_ptr<AScheduler>> Schedulers;
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
	//get most recent process
	std::shared_ptr<Process> getMostRecentProcess();
	//get current scheduler
	AScheduler* getScheduler();
	int checkCoreAvailability(int index);
	std::shared_ptr<SchedulerWorker> getCPUWorker(int index);
private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	Schedulers scheduler;
	CPUWorkers cpuWorkers;

	int coreCount;

	std::vector<std::shared_ptr<Process>> processList;
};
#pragma once
#include "AScheduler.h"
#include "SchedulerWorker.h"
#include "SchedulerWorkerRR.h"
#include "FCFSScheduler.h"
class GlobalScheduler  
{
public:
	typedef std::vector<std::shared_ptr<SchedulerWorker>> CPUWorkers;
	typedef std::vector<std::shared_ptr<SchedulerWorkerRR>> CPUWorkersRR;

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
	std::shared_ptr<AScheduler> getScheduler();
	bool checkCoreAvailability(int index);
	std::shared_ptr<SchedulerWorker> getCPUWorker(int index);
	std::shared_ptr<SchedulerWorkerRR> getCPUWorkerRR(int index);
	CPUWorkers& getCPUWorkers();
	CPUWorkersRR& getCPUWorkersRR();
	
	int availableCores();
	void setCoreCount(int coreCount);
	int getCoreCount() const;

private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	std::shared_ptr<AScheduler> scheduler;
	CPUWorkers cpuWorkers;
	CPUWorkersRR cpuWorkersRR;

	int coreCount = 4;

	std::vector<std::shared_ptr<Process>> processList;
};
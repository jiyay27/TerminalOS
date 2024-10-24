#pragma once
#include "AScheduler.h"

class GlobalScheduler
{
public:
	typedef std::unordered_map<String, std::shared_ptr<AScheduler>> CPUWorkers;

	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();

	void tick();

	void addProcess(std::shared_ptr<Process> process);
	int getProcessCount() const;
	std::shared_ptr<Process> getProcessName(int index);


private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	CPUWorkers cpuWorkers;
	std::vector<std::shared_ptr<Process>> processList;
};
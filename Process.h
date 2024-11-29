#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ICommand.h"
#include "TypedefRepo.h"

class Process
{
public:
	enum ProcessState
	{
		READY,
		RUNNING,
		WAITING,
		FINISHED,
	};
	
	Process(String name);
	Process(int pid, String name, int remainingInstructions);
	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand() const;
	void moveToNextLine();
	int getID() const;
	int getCPUCoreID() const;
	int getCommandCounter() const;
	int decrementCommandCounter();
	int getCommandListCount() const;
	ProcessState getState() const;
	String getName() const;
	void executeInstruction();
	bool isFinished() const;
	void setCoreID(int coreID);
	void setState(ProcessState state);

	//memory related
	size_t getMemoryRequired();
	void setAllocationState(bool state);
	bool getAllocationState();
	void setAssignedAt(void* ptr);
	void* getAssignedAt();

	void setAssignedAtVec(void* ptr);
	std::vector<void*> getAssignedAtVec();

private:
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;
	int commandCounter = 0;
	int cpuCoreID = -1;
	ProcessState currentState;

	//memory related
	size_t memoryRequired;
	bool memoryAllocated = false;
	void* assignedAt;
	std::vector<void*> assignedAtVec;
};
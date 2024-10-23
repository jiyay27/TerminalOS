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
	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand() const;
	void moveToNextLine();
	int getID() const;
	String getName() const;
	void executeInstruction();

private:
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;
	int commandCounter = 0;
	int cpuCoreID = -1;
	ProcessState currentState;

};
#pragma once

#include <string>
#include <memory>
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

	Process(int pid, String name);
	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand() const;
	void moveToNextLine();
	int getID() const;
	String getName() const;
	void executeInstruction();

private:
	int pid;
	String name;

	// COMMAND STUFF
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;
	
	int commandCounter;
	int cpuCoreID = -1;
	ProcessState currentState;

};
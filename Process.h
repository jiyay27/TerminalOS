#pragma once

#include <string>
#include <memory>

typedef std::string String;

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
	//Process (const std::string& name, int id, int numInstructions)
	// : processName(name), id(pid), totalInstructions(numInstructions), remainingInstructions(numInstructions) {}

	// INSTRUCTION FUNCTIONS
	int getID() const;
	String getProcessName() const;
	int getCurrentLine() const;

	void executeInstruction();
	int getRemainingInstructions() const;
	bool hasFinished() const;


	int pid = 0;
	String processName;
	int currentLine;

	int totalInstructions;
	int remainingInstructions;

	// COMMAND STUFF
	//typedef std::vector<std::sharedptr<ICommand>> CommandList;
	//CommandList commandList;
	
	//int commandCounter;
	//int cpuCoreID = -1;


	
};
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

	Process();

	Process(String name);

	int getID() const;
	String getProcessName() const;
	int getCurrentLine() const;

	void executeInstruction();
	int getRemainingInstructions() const;
	bool hasFinished() const;


	int pid;
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
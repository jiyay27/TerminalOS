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

	int getID() const;
	String getProcessName() const;
	int getCurrentLine() const;

	int pid = 0;
	String processName = NULL;
	int currentLine;

	// COMMAND STUFF
	//typedef std::vector<std::sharedptr<ICommand>> CommandList;
	//CommandList commandList;
	
	//int commandCounter;
	//int cpuCoreID = -1;

};
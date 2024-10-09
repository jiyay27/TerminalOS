#include "Process.h"

#include <string>

Process::Process()
{
	if (this->pid != 0)
		this->pid = Process::getID() + 1;
}

Process::Process(String processName)
{
	this->processName = processName;
}

int Process::getID()
{
	return this->pid;
}

String Process::getProcessName()
{
	return this->processName;
}

int Process::getCurrentLine()
{
	return this->currentLine;
}
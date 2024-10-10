#include "Process.h"

#include <string>

Process::Process(String processName)
{
	this->pid = this->pid + 1;
	this->processName = processName;
	this->currentLine = 0;
}

int Process::getID() const
{
	return this->pid;
}

String Process::getProcessName() const
{
	return this->processName;
}

int Process::getCurrentLine() const
{
	return this->currentLine;
}
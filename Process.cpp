#include "Process.h"

#include <string>

Process::Process()
{
	this->pid = this->pid + 1;
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
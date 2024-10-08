#include "Process.h"

#include <string>

Process::Process()
{
	if (this->pid != 0)
		this->pid = Process::getID() + 1;
}

Process::Process(String processName)
{

}

int Process::getID()
{
	return 0;
}

String Process::getProcessName()
{
	return "a";
}

int Process::getCurrentLine()
{
	return 0;
}
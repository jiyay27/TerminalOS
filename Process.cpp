#include "Process.h"

#include <string>
#include <iostream>
#include <cstdlib>

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

void Process::executeInstruction()
{
	if(this->remainingInstructions > 0){
		std::cout << "Executing instruction for Process" << this->pid << ": " << this->processName << "\n";
		this->remainingInstructions--;
	}else{
		std::cout << "Process" << this->pid << ": " << this->processName << " has already finished.\n";

	}
}

int Process::getRemainingInstructions() const{
	return remainingInstructions;
}

bool Process::hasFinished() const{
	return this->remainingInstructions == 0;
}
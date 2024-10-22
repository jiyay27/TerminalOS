#include "Process.h"
#include "ConsoleManager.h"

#include <string>
#include <iostream>
//#include <cstdlib>

Process::Process()
	: pid(0), processName("Unknown"), currentLine(0), totalInstructions(0), remainingInstructions(0) {}


Process::Process(String name)
{
	this->pid = 0;
	this->processName = name;
	//this->pid = ConsoleManager::getInstance()->getProcessCount();
	this->currentLine = 0;
	this->totalInstructions = 0;
	this->remainingInstructions = 50;
	//TODO: print commands
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
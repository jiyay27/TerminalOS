#include "Process.h"
#include "ConsoleManager.h"
#include "PrintCommand.h"
#include <string>
#include <iostream>



Process::Process(String name)
{
	this->pid = 0;
	this->name = name;
	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
}

int Process::getID() const
{
	return this->pid;
}

void Process::addCommand(ICommand::CommandType commandType)
{
	String toPrint = "Command added";
	const std::shared_ptr<ICommand> command = std::make_shared<PrintCommand>(this->pid, toPrint);
	this->commandList.push_back(command);
}

//called by CPU Core Worker
void Process::executeCurrentCommand() const
{
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine()
{
	this->commandCounter++;
}

void Process::executeInstruction()
{
	if (this->commandCounter < this->commandList.size())
	{
		this->executeCurrentCommand();
		this->moveToNextLine();
	}
	else
	{
		this->currentState = ProcessState::FINISHED;
	}
}

bool Process::isFinished() const
{
	return this->commandCounter == this->commandList.size();
}

String Process::getName() const
{
	return this->name;
}



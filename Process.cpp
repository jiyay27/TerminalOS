#include "Process.h"
#include "ConsoleManager.h"
#include "PrintCommand.h"
#include <string>
#include <iostream>
#include "GlobalScheduler.h"

Process::Process(String name)
{
	srand(time(0));
	int min = 1000;
	int max = 1500;
	int randomNum = min + rand() % (max - min + 1);

	this->pid = GlobalScheduler::getInstance()->getProcessCount();
	this->name = name;
	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
	for (int i = 0; i < randomNum; i++)
	{
		this->addCommand(ICommand::CommandType::PRINT);
	}
}

Process::Process(int pid, String name, int remainingInstructions)
{
	this->pid = pid;
	this->name = name;
	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
	for (int i = 0; i < remainingInstructions; i++)
	{
		this->addCommand(ICommand::CommandType::PRINT);
	}
}

int Process::getID() const
{
	return this->pid;
}

int Process::decrementCommandCounter()
{
	return --this->commandCounter;
}

int Process::getCommandCounter() const
{
	return this->commandCounter;
}

int Process::getCommandListCount() const
{
	return this->commandList.size();
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

String Process::getName() const
{
	return this->name;
}

bool Process::isFinished() const
{
	return this->commandCounter >= this->commandList.size();
}

void Process::addCommand(ICommand::CommandType commandType)
{
	String toPrint = "Command added";
	const std::shared_ptr<ICommand> command = std::make_shared<PrintCommand>(this->pid, toPrint);

	// Check if the command is a null pointer
	if (command == nullptr)
	{
		std::cerr << "Failed to create command. Null pointer detected." << std::endl;
		return; // Exit the function if the command is null
	}

	this->commandList.push_back(command);
	//std::cout << "Command added to process " << this->name << std::endl;
}


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
		decrementCommandCounter();
		this->currentState = ProcessState::FINISHED;
	}
}

void Process::setCoreID(int coreID)
{
	this->cpuCoreID = coreID;
}

void Process::setState(ProcessState state)
{
	this->currentState = state;
}


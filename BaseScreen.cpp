#include "BaseScreen.h"
#include "ConsoleManager.h"
String responseBase = "";
String outputMessageBase = "";

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName)
{
	process = std::make_shared<Process>(processName);
	this->attachedProcess = process;
}

void BaseScreen::onEnabled()
{
	system("cls");
	this->printProcessInfo();
}

void BaseScreen::display()
{
	if (outputMessageBase.empty())
	{
		onEnabled();
	}

	if (outputMessageBase == "exit")
	{
		ConsoleManager::getInstance()->returnToPreviousConsole();
		outputMessageBase = "";
	}

	if (outputMessageBase == "clear")
	{
		system("cls");
		outputMessageBase = "";
	}

	if (outputMessageBase == "process-smi")
	{
		this->printProcessInfo();
		outputMessageBase = "";
	}
}

void BaseScreen::process()
{
	if (this->refreshed == false) {
		this->refreshed = true;
	}

	std::cout << "root:\\> ";
	getline(std::cin, responseBase); // get user input
	
	if (responseBase == "exit")
	{
		outputMessageBase = responseBase;
	}
	else if (responseBase == "clear")
	{
		outputMessageBase = responseBase;
	}
	else if (responseBase == "process-smi") 
	{
		outputMessageBase = responseBase;
	}
	else 
	{
		std::cout << "Unknown command: " << responseBase << std::endl;
	}
}

String BaseScreen::getName() const
{
	return this->attachedProcess->getName();
}

std::shared_ptr<Process> BaseScreen::getProcess() const
{
	return this->attachedProcess;
}

void BaseScreen::printProcessInfo() const
{
	if (this->attachedProcess->isFinished())
	{
		std::cout << "" << std::endl;
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getID() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Finished!"<< std::endl;
		std::cout << "" << std::endl;
	}
	else 
	{
		std::cout << "" << std::endl;
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getID() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << this->attachedProcess->getCommandListCount() << std::endl;
		std::cout << "" << std::endl;
	}
}
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
		ConsoleManager::getInstance()->returnToPreviousConsole();
	}

	else if (responseBase == "clear")
	{
		system("cls");
	}
	else if (responseBase == "process-smi") {
		this->printProcessInfo();
	}
	else {
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
	std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
	std::cout << "ID: " << this->attachedProcess->getID() << std::endl;
	std::cout << "" << std::endl;
	//std::cout << "Current insruction line: " << this->attachedProcess->getCurrentLine() << std::endl;
}
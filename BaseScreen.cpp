#include "BaseScreen.h"

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
	printProcessInfo();
}

void BaseScreen::display()
{
	printProcessInfo();
	if (outputMessageBase == "0")
	{

	}
}

void BaseScreen::process()
{
	std::cout << "root:\\> ";
	getline(std::cin, responseBase); // get user input
	
	if (responseBase == "exit")
	{
		return;
	}

	if (responseBase == "clear")
	{
		system("cls");
	}
}

std::shared_ptr<Process> BaseScreen::getProcess() const
{
	return this->attachedProcess;
}
void BaseScreen::printProcessInfo() const
{
	std::cout << "Process: " << this->attachedProcess->getProcessName() << std::endl;
	std::cout << "ID: " << this->attachedProcess->getID() << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Current insruction line: " << this->attachedProcess->getCurrentLine() << std::endl;
}
#include "BaseScreen.h"

String response = "";
String outputMessage = "";

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName)
{
	this->attachedProcess = std::make_shared<Process>(processName);
}

void BaseScreen::onEnabled()
{
	system("cls");
	printProcessInfo();
}

void BaseScreen::display()
{

}

void BaseScreen::process()
{
	std::cout << "root:\\> ";
	getline(std::cin, response); // get user input
	
	if (response == "exit")
	{
		return;
	}

	if (response == "clear")
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
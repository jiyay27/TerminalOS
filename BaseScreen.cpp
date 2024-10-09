#include "BaseScreen.h"

#include "Process.h"
#include <iostream>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName)
{
	onEnabled();

}

void BaseScreen::onEnabled()
{
	system("cls");
	this->refreshed = true;
}

void BaseScreen::process()
{

}

void BaseScreen::display()
{

}

void BaseScreen::printProcessInfo() const
{
	std::cout << "Process: " << this->attachedProcess << std::endl;
	std::cout << "ID: " << this->attachedProcess << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Current insruction line: " << this->attachedProcess << std::endl;
}
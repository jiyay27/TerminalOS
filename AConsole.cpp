#include "AConsole.h"

#include <string>

AConsole::AConsole()
{
}

AConsole::AConsole(String name)
{
	this->name = name;
}

std::string AConsole::getName() const
{
	return this->name;
}
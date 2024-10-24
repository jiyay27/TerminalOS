#include "PrintCommand.h"

#include <iostream>
#include <string>
#include <sstream> // Include this header for std::stringstream
#include "TypedefRepo.h"
#include "IETThread.h"
//#include "MessageBuffer.h"

PrintCommand::PrintCommand(int pid, String& toPrint) : ICommand(pid, PRINT)
{
    this->toPrint = toPrint;
}

void PrintCommand::execute()
{
    ICommand::execute();
    std::stringstream msg; 
    msg << "PID " << this->pid << ":" << this->toPrint << std::endl; // Use std::string instead of String
}
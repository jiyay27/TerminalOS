#pragma once
//#include "GlobalConfig.h"
#include "IETThread.h"
#include <string>
#include "TypedefRepo.h"

class ICommand
{
public:
    enum CommandType
    {
        IO,
        PRINT
    };

    ICommand(int pid, CommandType commandType);
    CommandType getCommandType();
    virtual void execute();

protected:
    int pid;
    CommandType commandType;
};

inline ICommand::CommandType ICommand::getCommandType()
{
    return this->commandType;
};

inline void ICommand::execute()
{
    IETThread::sleep(10);
}

inline ICommand::ICommand(int pid, CommandType commandType)
{
    this->pid = pid;
    this->commandType = commandType;
}
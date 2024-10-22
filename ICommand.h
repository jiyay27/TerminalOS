#pragma once
//#include "GlobalConfig.h"
//#include "IETThread.h"
#include <string>
#include "TypedefRepo.h"
class ICommand
{
public:
    enum CommandType
    {
        PRINT
    };

    ICommand(int pid, CommandType commandType);
    CommandType getCommandType();
    virtual void execute();

protected:
    int pid;
    CommandType commandType;

inline ICommand::CommandType ICommand::getCommandType()
{
    return this->commandType;
};

};
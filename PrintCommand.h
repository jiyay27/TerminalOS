#pragma once
#include "ICommand.h"
#include <string>
//#include "TypedefRepo.h"

class PrintCommand : public ICommand
{
public:
    PrintCommand(int pid, String& toPrint);
    void execute() override;
private:
    String toPrint;

};

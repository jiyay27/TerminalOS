#pragma once
#include "ICommand.h"
#include <string>
#include "TypedefRepo.h"

class PrintCommand : public ICommand
{
public:
    PrintCommand(int pid, String& toPrint);
    ~PrintCommand() = default;
    void execute() override;
private:
    String toPrint;

};

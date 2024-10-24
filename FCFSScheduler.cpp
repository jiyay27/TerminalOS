#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{
    this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

void FCFSScheduler::addProcess(const Process& process, int core) 
{
     if(core >=0 && core < numCores)
     {
        processQueues[core].push_back(process);
     } 
     else 
     {
        std::cerr << "Invalid core specified for process addition.\n";
     }
}

//assign coreworker to process
void FCFSScheduler::assignCore(std::shared_ptr<Process> process) 
{
    if (process->getState() == Process::READY)
    {
        for (int core = 0; core < numCores; ++core)
        {
            if (GlobalScheduler::getInstance()->checkCoreAvailability(core) >= 0)
            {
                process->setCoreID(core);
                process->setState(Process::RUNNING);
                GlobalScheduler::getInstance()->getCPUWorker(core)->addProcess(process->getID());
                GlobalScheduler::getInstance()->getCPUWorker(core)->isOccupied();
            }
        }
    }
    else
    {
        std::cerr << "Process is not in READY state.\n";
    }
}

void FCFSScheduler::execute() 
{
    while (!processQueues[0].empty())
    {
        for (int core = 0; core < numCores; ++core)
        {
            if (!processQueues[core].empty())
            {
                Process currentProcess = processQueues[core].back();
                processQueues[core].pop_back();

                while (!currentProcess.isFinished())
                {
                    currentProcess.executeInstruction();
                }

                std::cout << "Process" << currentProcess.getName() << "completed on Core" << core + 1 << "." << std::endl;
            }
        }
    }
}

FCFSScheduler::FCFSScheduler(int cores)
{
    this->numCores = cores;
}
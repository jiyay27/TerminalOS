#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{
    //this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
     if(core >=0)
     {
        processQueues[core].push_back(process);
     } 
     else 
     {
        std::cerr << "Invalid core specified for process addition.\n";
     }
}

// TODO: refactor
int FCFSScheduler::assignCore(std::shared_ptr<Process> process) 
{
     for (int core = 0; core < cores; core++)
     {
         std::cout << "Assign core func for loop." << std::endl;
         if(GlobalScheduler::getInstance()->checkCoreAvailability(core)==true)
         {
             std::cout << "Assign core func." << std::endl;
             process->setCoreID(core);
             process->setState(Process::RUNNING);
             GlobalScheduler::getInstance()->getCPUWorker(core)->addProcess(process);
             GlobalScheduler::getInstance()->getCPUWorker(core)->isOccupied();
			 FCFSScheduler::addProcess(process, core);
             return core;
         }
     }
}

void FCFSScheduler::execute() 
{
    for (int i = 0; i < numCores; i++)
    {
        GlobalScheduler::getInstance()->getCPUWorker(i)->run();
    }
    //while (!processQueues[0].empty())
    //{
    //    for (int core = 0; core < numCores; ++core)
    //    {
    //        if (!processQueues[core].empty())
    //        {
    //            Process currentProcess = processQueues[core].back();
    //            //processQueues[core].pop_back(core)
    //            processQueues[core].erase(processQueues[core].begin());

    //            while (!currentProcess.isFinished())
    //            {
    //                currentProcess.executeInstruction();
    //            }
    //            std::cout << "Process" << currentProcess.getName() << "completed on Core" << core + 1 << "." << std::endl;
    //        }
    //    }
    //}
}

FCFSScheduler::FCFSScheduler()
{
    this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

FCFSScheduler::FCFSScheduler(int numCores)
{
    this->numCores = numCores;
}

void FCFSScheduler::printCores() {
	std::cout << "Number of cores: " << this->numCores << std::endl;
}
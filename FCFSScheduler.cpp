#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{
    //this->numCores = GlobalScheduler::getInstance()->getCoreCount();
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core)
{
     if(core >=0 && core < numCores)
     {
        //processQueues[core].push_back(process);
		 std::cout << "WOW!" << std::endl;
     } 
     else 
     {
        std::cerr << "Invalid core specified for process addition.\n";
     }
}

// TODO: refactor
int FCFSScheduler::assignCore(std::shared_ptr<Process> process) 
{
     for (int core = 0; core < numCores; ++core)
     {
         if(GlobalScheduler::getInstance()->checkCoreAvailability(core) != -1)
         {
             process->setCoreID(core);
             process->setState(Process::RUNNING);
             GlobalScheduler::getInstance()->getCPUWorker(core)->addProcess(process->getID());
             GlobalScheduler::getInstance()->getCPUWorker(core)->isOccupied();
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
   this->numCores = 4;
}

void FCFSScheduler::printCores() {
	std::cout << "Number of cores: " << this->numCores << std::endl;
}
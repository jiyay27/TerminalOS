#include "FCFSScheduler.h"
#include <algorithm>

void FCFSScheduler::init()
{

}

void FCFSScheduler::addProcess(const Process& process, int core) 
{
     if(core >=0 && core < numCores)
     {
          //processQueues[core].push_back(process);
     } 
     else 
     {
        std::cerr << "Invalid core specified for process addition.\n";
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
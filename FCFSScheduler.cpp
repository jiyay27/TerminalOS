#include "FCFSScheduler.h"
#include <algorithm>

void FCFSScheduler::initialize() const
{

}

void FCFSScheduler::addProcess(Process& process, int core = 0) const
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

void FCFSScheduler::sortProcessQueues() const
{
     for (auto& queue : processQueues)
     {
        std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) 
        {
            return a.getRemainingInstructions() > b.getRemainingInstructions();
        });
     }
}


void FCFSScheduler::runScheduler() const
{
    while (!processQueues[0].empty())
    {
        for (int core = 0; core < numCores; ++core)
        {
            if (!processQueues[core].empty())
            {
                Process currentProcess = processQueues[core].back();
                //processQueues[core].pop_back();

                while (!currentProcess.isFinished())
                {
                    currentProcess.executeInstruction();
                }

                std::cout << "Process" << currentProcess.getRemainingInstructions() << "completed on Core" << core + 1 << "." << std::endl;
            }
        }
    }
}
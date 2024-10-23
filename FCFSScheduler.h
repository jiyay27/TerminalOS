#pragma once

#include <iostream>
#include <vector>
#include "Process.h"

class FCFSScheduler
{
    private:
        int numCores;
        std::vector<std::vector<Process>> processQueues;

    public:
        FCFSScheduler(int cores) : numCores(cores), processQueues(cores){}

        void initialize() const;
        void addProcess() const; // add process to core worker
        void sortProcessQueues() const;
        void runScheduler() const;



        /*void addProcess(const Process& process, int core = 0) {
            if(core >=0 && core < numCores){
                processQueues[core].push_back(process);
            }else {
                std::cerr << "Invalid core specified for process addition.\n";
            }
        }*/

       /* void sortProcessQueues(){
            for (auto& queue : processQueues){
                std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
                    return a.getRemainingInstructions() > b.getRemainingInstructions();
                });
            }
        }

        void runScheduler{
            while(!processQueues[0].empty()){
                for (int core = 0; core < numCores; ++core){
                    if(!processQueues[core.empty]()){
                        Process currentProcess = processQuees[core].back();
                        processQueues[core].pop_back();

                        while(!currentProcess.hasFinished()){
                            currentProcess.executeInstruction();
                        }
                        std::cout < "Process" << currentProcess.getRemainingInstructions() << "completed on Core" << core + 1 << ".\n";
                    }
                }
            }
        }*/




};

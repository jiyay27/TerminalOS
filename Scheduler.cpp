#include <iostream>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <atomic>

class Process {
public:
    void setCPUCoreID(int id) { cpuCoreID = id; }
    int getCPUCoreID() const { return cpuCoreID; }
private:
    int cpuCoreID;
};

class CPUWorker {
public:
    bool isCPUProcessing() const { return processing; }
    void setCurrentProcess(std::shared_ptr<Process> process) {
        currentProcess = std::move(process);
        processing = (currentProcess != nullptr);
    }
    int getID() const { return workerID; }
private:
    bool processing = false;
    int workerID;
    std::shared_ptr<Process> currentProcess;
};

class Scheduler {
public:
    void addToProcessList(std::shared_ptr<Process> process);
    void addToCPUWorkers(std::shared_ptr<CPUWorker> worker);
    void runScheduler();
    void tryAssignProcesses();
    std::shared_ptr<CPUWorker> findAvailableWorker();
    void assignProcessToWorker(std::shared_ptr<CPUWorker> worker);
    int checkCPUWorkers() const;
    void turnOff();

private:
    // Definitions for listOfProcesses and listOfCPUWorkers
    std::list<std::shared_ptr<Process>> listOfProcesses;
    std::list<std::shared_ptr<CPUWorker>> listOfCPUWorkers;

    std::atomic<bool> isOn{ true };
    std::thread schedulerThread;

    std::mutex processMutex;
    mutable std::mutex workerMutex;

};

void Scheduler::addToProcessList(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(processMutex);
    listOfProcesses.push_back(std::move(process));
}

void Scheduler::addToCPUWorkers(std::shared_ptr<CPUWorker> worker) {
    std::lock_guard<std::mutex> lock(workerMutex);
    listOfCPUWorkers.push_back(std::move(worker));
}

void Scheduler::runScheduler() {
    schedulerThread = std::thread([this]() {
        while (isOn) {
            tryAssignProcesses();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        });

    // No detaching to ensure we can join the thread on shutdown
}

void Scheduler::tryAssignProcesses() {
    std::lock_guard<std::mutex> processLock(processMutex);  // Lock the process mutex
    std::lock_guard<std::mutex> workerLock(workerMutex);    // Lock the worker mutex

    while (!listOfProcesses.empty() && checkCPUWorkers() > 0) {
        auto availableWorker = findAvailableWorker();
        if (availableWorker != nullptr) {
            assignProcessToWorker(availableWorker);
        }
    }
}


std::shared_ptr<CPUWorker> Scheduler::findAvailableWorker() {
    for (auto& worker : listOfCPUWorkers) {
        if (!worker->isCPUProcessing()) {
            return worker;  // Return the first available worker
        }
    }
    return nullptr;
}

void Scheduler::assignProcessToWorker(std::shared_ptr<CPUWorker> worker) {
    if (!listOfProcesses.empty()) {
        auto process = std::move(listOfProcesses.front());
        listOfProcesses.pop_front();
        process->setCPUCoreID(worker->getID());
        worker->setCurrentProcess(std::move(process));
    }
}

int Scheduler::checkCPUWorkers() const {
    std::lock_guard<std::mutex> lock(workerMutex);  // Lock the worker list during this operation
    return std::count_if(listOfCPUWorkers.begin(), listOfCPUWorkers.end(),
        [](const auto& worker) { return !worker->isCPUProcessing(); });
}

void Scheduler::turnOff() {
    isOn = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();  // Ensure the thread is joined before exiting
    }
}

#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "Config.h"
#include <iomanip>
#include <fstream>
#include <filesystem>
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
string response = "";
string outputMessage = "";
string outputArg2 = "";
int isInitialized = 0;

MainConsole::MainConsole() 
{
    this->name = "MainConsole";
}

MainConsole::MainConsole(String name) //constructor
{
    this->name = name;
}

void MainConsole::onEnabled() //main screen start up
{
    system("cls");
    asciiart();
    header();
}

void MainConsole::display() // Displays output
{
    if (outputMessage.empty()) 
    {
        onEnabled();
    }

    if (isInitialized == 1) {
        if (outputMessage == "initialize") {
            config.setParamList(this->fileName);
			outputMessage = "";

        }

        if (outputMessage == "clear")
        {
            system("cls");
        }

        if (outputMessage == "exit")
        {
            outputMessage = "";
            for (size_t i = 0; i < GlobalScheduler::getInstance()->getCoreCount(); i++)
            {
                GlobalScheduler::getInstance()->getCPUWorker(i)->stop();
				GlobalScheduler::getInstance()->getCPUWorkerRR(i)->stop();
            }
			GlobalScheduler::getInstance()->getScheduler()->stop();
            ConsoleManager::getInstance()->exitConsole();
        }

        if (outputMessage == "screenr") 
        {
            outputMessage = "";
            ConsoleManager::getInstance()->switchToScreen(outputArg2);
        }

        if (outputMessage == "screens")
        {
            outputMessage = "";
            ConsoleManager::getInstance()->createBaseScreen(outputArg2);
            int newCore = GlobalScheduler::getInstance()->getScheduler()->checkCoreQueue();
            GlobalScheduler::getInstance()->getScheduler()->assignCore(GlobalScheduler::getInstance()->getMostRecentProcess(), newCore);
        }

        if (outputMessage == "screenls")
        {
            std::cout << this->displayCPUUtil();
            std::cout << this->displayRunning();
            std::cout << this->displayFinished();
        }

		if (outputMessage == "sched-test")
		{
            outputMessage = "";
            GlobalScheduler::getInstance()->getScheduler()->schedulerStart();
		}
        //stops scheduler test
        if (outputMessage == "sched-stop")
        {
            GlobalScheduler::getInstance()->getScheduler()->schedulerStop();
			outputMessage = "";
        }
		//puts displayCPUUtil, displayRunning, displayFinished into a txt file
        if (outputMessage == "report-util")
        {
            std::ofstream outFile("csopesy-log.txt");
            if (outFile.is_open())
            {
                outFile << this->displayCPUUtil();
                outFile << this->displayRunning();
                outFile << this->displayFinished();
                outFile.close();
                std::cout << "Report generated: report.txt" << std::endl;
                std::cout << "File path: " << std::filesystem::current_path() / "csopesy-log.txt" << std::endl;
            }
            else
            {
                std::cerr << "Unable to open file for writing." << std::endl;
            }
            outputMessage = "";
        }

		if (outputMessage == "process-smi")
		{
            std::cout << this->displayProcessSMI();
			outputMessage = "";
		}

        if (outputMessage == "vmstat")
        {
            //display memory things
			std::cout << this->displayVMStat();
        }
        if (outputMessage == "invalid")
        {
			outputMessage = "";
			std::cout << "Invalid command..." << std::endl;
        }
    }
    else
        cout << "Console has not been initialized." << endl;
}

void MainConsole::process() // Takes in input and processes it
{
    cout << "root:\\>";
    getline(cin, response);  // Get the user input

    stringstream ss(response);
    string command, arg1, arg2;
    ss >> command >> arg1 >> arg2;  // Parse the input

    if (command == "clear") {
        outputMessage = command;
    }
    else if (command == "exit") {
        outputMessage = command;
    }
    else if (command == "initialize") {
        outputMessage = command;
        isInitialized = 1;
    }
    else if (command == "scheduler-test") {
        outputMessage = "sched-test";
    }
    else if (command == "scheduler-stop") {
        outputMessage = "sched-stop";
    }
    else if (command == "report-util") {
        outputMessage = "report-util";
    }
    else if (command == "process-smi") {
        outputMessage = "process-smi";
    }
    else if (command == "screen" && arg1 == "-r" && !arg2.empty()) {
        outputMessage = "screenr";
        outputArg2 = arg2;
    }
    else if (command == "screen" && arg1 == "-s" && !arg2.empty()) {
        outputMessage = "screens";
        outputArg2 = arg2;
    }
    else if (command == "screen" && arg1 == "-ls") {
        outputMessage = "screenls";
	}
	else if (command == "vmstat") {
		outputMessage = "vmstat";
	}
    else {
        outputMessage = "invalid";
    }
}

void MainConsole::asciiart() const {
    cout << "  _____  _____  ____  _____  ______  _______     __ \n";
    cout << " / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / / \n";
    cout << "| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ /  \n";
    cout << "| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /   \n";
    cout << "| |____ ____) | |__| | |    | |____ ____) |  | |    \n";
    cout << " \\_____|_____/ \\____/|_|    |______|_____/   |_|    \n";
}

void MainConsole::header() const {
    SetConsoleTextAttribute(hConsole, 10);
    cout << "Hello, Welcome to CSOPESY commandline!\n";
    SetConsoleTextAttribute(hConsole, 14);
    cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    SetConsoleTextAttribute(hConsole, 15);
}

String MainConsole::displayCPUUtil() const
{
    int coresUsed = this->computeCoresUsed();
    float cpuUtil = this->computeCoreUtil();

    std::ostringstream oss;
    oss << "\nCPU Utilization: " << cpuUtil << "%\n";
    oss << "Cores used: " << coresUsed << "\n";
    oss << "Cores available: " << this->computeCoresAvail() << "\n\n";
    return oss.str();
}

String MainConsole::displayRunning()
{
    String truncatedProcName;
    std::ostringstream oss;
    oss << "Running processes:\n";
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (!GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            truncatedProcName = GlobalScheduler::getInstance()->getProcess(i)->getName();
            truncatedProcName = truncateRightLine(truncatedProcName, 10);
            oss << std::left
                << std::setw(12) << truncatedProcName
                << std::setw(4) << "Core: " << GlobalScheduler::getInstance()->getProcess(i)->getCPUCoreID()
                << std::right << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter()
                << "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

String MainConsole::displayFinished()
{
    String truncatedProcName;
    std::ostringstream oss;
    oss << "Finished processes:\n";
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            truncatedProcName = GlobalScheduler::getInstance()->getProcess(i)->getName();
            truncatedProcName = truncateRightLine(truncatedProcName, 10);
            oss << std::left
                << std::setw(12) << truncatedProcName
                << std::setw(4) << "FINISHED"
                << std::right << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter()
                << "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

std::string MainConsole::displayProcessSMI()
{
    String truncatedProcName;

    std::ostringstream oss;
    oss << "\n-----------------------------------------\n";
    oss << "| PROCESS-SMI V1.00 Driver Version 1.00 |\n";
    oss << "-----------------------------------------\n";

    oss << std::left
    << std::setw(5) << "CPU-Util: " << this->computeCoreUtil() << "%\n"
    << std::setw(5) << "Memory Usage: " << std::right
    << std::setw(2)<< (float)convertKbToMb(this->computeMemoryUsed()) << "MiB" << " / " << (float)convertKbToMb(this->getMaxSize()) << "MiB\n"
	<< std::setw(5) << "Memory Util: " << this->computeMemoryUtil() << "%\n";

    oss << "-----------------------------------------\n";
    oss << " Running processes and memory usage:     \n";
    oss << "-----------------------------------------\n";

    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (!GlobalScheduler::getInstance()->getProcess(i)->isFinished() && GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() > 0)
        {
            truncatedProcName = GlobalScheduler::getInstance()->getProcess(i)->getName();
            truncatedProcName = truncateRightLine(truncatedProcName, 10);
            oss << std::left
                << std::setw(12) << truncatedProcName
                << (float)convertKbToMb(GlobalScheduler::getInstance()->getProcess(i)->getMemoryRequired()) << "MiB" << "\n";
        }
    }
    
    oss << "-----------------------------------------\n" << std::endl;

    return oss.str();
}

float MainConsole::computeCoreUtil() const
{
    int coreTotal;
    int coresAvail;

    if (GlobalScheduler::getInstance()->getScheduler()->getName() == "FCFS")
    {
        coreTotal = GlobalScheduler::getInstance()->getCPUWorkers().size();
        coresAvail = GlobalScheduler::getInstance()->availableCores();
    }
    else
    {
        coreTotal = GlobalScheduler::getInstance()->getCPUWorkersRR().size();
        coresAvail = GlobalScheduler::getInstance()->availableCoresRR();
    }

    int coresUsed = coreTotal - coresAvail;
    float cpuUtil = coresUsed * 100 / coreTotal;

    return cpuUtil;
}

int MainConsole::computeCoresUsed() const
{
    int coreTotal;
    int coresAvail;

    if (GlobalScheduler::getInstance()->getScheduler()->getName() == "FCFS")
    {
        coreTotal = GlobalScheduler::getInstance()->getCPUWorkers().size();
        coresAvail = GlobalScheduler::getInstance()->availableCores();
    }
    else
    {
        coreTotal = GlobalScheduler::getInstance()->getCPUWorkersRR().size();
        coresAvail = GlobalScheduler::getInstance()->availableCoresRR();
    }

    int coresUsed = coreTotal - coresAvail;

    return coresUsed;
}

int MainConsole::computeCoresAvail() const
{
    int coresAvail;

    if (GlobalScheduler::getInstance()->getScheduler()->getName() == "FCFS")
    {
        coresAvail = GlobalScheduler::getInstance()->availableCores();
    }
    else
    {
        coresAvail = GlobalScheduler::getInstance()->availableCoresRR();
    }

    return coresAvail;
}


float MainConsole::computeMemoryUtil() const
{
    Config config;
    config.setParamList("config.txt");

    size_t memoryUtil;

    if (config.getMaxMem() == config.getMemFrame())
    { // calculate for flat
		auto allocator = FlatMemoryAllocator::getInstance();
        memoryUtil = allocator->computeMemoryUtil();
        return (float)memoryUtil;
    }
    else
    { // calculate for paging
        //auto allocator = FlatMemoryAllocator::getInstance();
        auto allocator = PagingAllocator::getInstance();
        memoryUtil = allocator->computeMemoryUtil();
        return (float)memoryUtil;
    }
}

int MainConsole::computeMemoryUsed() const
{
    Config config;
    config.setParamList("config.txt");

    size_t memoryUtil;
    int memoryUsed;

    if (config.getMaxMem() == config.getMemFrame())
    {
        auto allocator = FlatMemoryAllocator::getInstance();
        memoryUsed = allocator->computeMemoryUsed();
        return memoryUsed;
    }
    else
    {
        auto allocator = PagingAllocator::getInstance();
        memoryUsed = allocator->computeMemoryUsed();
        return (float)memoryUsed;
    }
}

int MainConsole::computeMemoryAvail() const
{
    Config config;
    config.setParamList("config.txt");

    size_t memoryUtil;
    int memoryUsed;
    int memoryAvail;

    if (config.getMaxMem() == config.getMemFrame())
    {
        auto allocator = FlatMemoryAllocator::getInstance();
        memoryAvail = allocator->computeMemoryAvail();
        return memoryAvail;
    }
    else
    {
        auto allocator = PagingAllocator::getInstance();
        memoryAvail = allocator->computeMemoryAvail();
        return memoryAvail;
    }
}

size_t MainConsole::getMaxSize() const
{
    Config config;
    config.setParamList("config.txt");

    size_t memoryUtil;
    int memoryUsed;
    int memoryAvail;

    if (config.getMaxMem() == config.getMemFrame())
    {
        size_t maxMem;
        auto allocator = FlatMemoryAllocator::getInstance();
        maxMem = allocator->getMaximumSize();
        return maxMem;
    }
    else
    {
        size_t maxMem;
        auto allocator = PagingAllocator::getInstance();
        maxMem = allocator->getMaximumSize();
        return maxMem;
    }
}

float MainConsole::convertKbToMb(size_t kb) const
{
	return (float)kb / 1024;
}

string MainConsole::getName() const 
{
    return this->name;
}


string MainConsole::truncateLeftLine(String str, int maxLength)
{
    if (str.length() > maxLength) {
        return "..." + str.substr(str.length() - (maxLength - 3));
    }
    return str;
}

string MainConsole::truncateRightLine(String str, int maxLength)
{
    if (str.length() > maxLength) {
        return str.substr(0, maxLength - 3) + "...";
    }
    return str;
}

string MainConsole::displayVMStat() 
{
	int idle = 0, active = 0, total = 0;
	std::stringstream oss;
    oss << "-----------------------------------------\n";
    oss << "                VMSTAT                   \n";
    oss << "-----------------------------------------\n";
    oss << std::left;
	oss << std::setw(5) << "Total Memory: " << this->getMaxSize() << "KB\n";
	oss << std::setw(5) << "Memory Used: " << this->computeMemoryUsed() << "KB\n";
	oss << std::setw(5) << "Memory Available: " << this->computeMemoryAvail() << "KB\n";
	for (int i = 0; i < GlobalScheduler::getInstance()->getCoreCount(); i++)
	{
		total += GlobalScheduler::getInstance()->getCPUWorkerRR(i)->getCPUClock();
		idle += GlobalScheduler::getInstance()->getCPUWorkerRR(i)->getIdleClock();
		active += total - idle;
	}
	oss << std::setw(5) << "Idle CPU Ticks: " << idle << "\n";
	oss << std::setw(5) << "Active CPU Ticks: " << active << "\n";
	oss << std::setw(5) << "Total CPU Ticks: " << total << "\n";
    oss << std::endl;
    //paging add 
	return oss.str();

}
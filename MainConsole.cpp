#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "Config.h"
#include <iomanip>
#include <fstream>
#include <filesystem>


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
            
            std::cout << this->displayCPUUtil(coresAvail, coreTotal);
            std::cout << this->displayRunning();
            std::cout << "" << std::endl;
            std::cout << this->displayFinished();
        }

		if (outputMessage == "sched-test")
		{
            srand(static_cast<unsigned int>(time(0)));
            for (int i = 1; i <= 10; ++i) {
                std::ostringstream oss;  
                oss << "proc-" << std::setw(2) << std::setfill('0') << i;
                std::string procName = oss.str();

                ConsoleManager::getInstance()->createBaseScreen2(procName);
                int newCore = GlobalScheduler::getInstance()->getScheduler()->checkCoreQueue();
                GlobalScheduler::getInstance()->getScheduler()->assignCore(GlobalScheduler::getInstance()->getMostRecentProcess(), newCore);
            }
			outputMessage = "";
		}
        //stops scheduler test
        if (outputMessage == "sched-stop")
        {
			outputMessage = "";
        }
		//puts displayCPUUtil, displayRunning, displayFinished into a txt file
        if (outputMessage == "report-util")
        {
            std::ofstream outFile("csopesy-log.txt");
            if (outFile.is_open())
            {
                int coreTotal = GlobalScheduler::getInstance()->getCPUWorkers().size();
                int coresAvail = GlobalScheduler::getInstance()->availableCores();
                outFile << this->displayCPUUtil(coresAvail, coreTotal);
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

std::string MainConsole::displayCPUUtil(int coresAvail, int coresTotal) const
{
    int coresUsed = coresTotal - coresAvail;
    float cpuUtil = coresUsed * 100 / coresTotal;
    std::ostringstream oss;
    oss << "\nCPU Utilization: " << cpuUtil << "%\n";
    oss << "Cores used: " << coresUsed << "\n";
    oss << "Cores available: " << coresAvail << "\n\n";
    return oss.str();
}

std::string MainConsole::displayRunning() const
{
    std::ostringstream oss;
    oss << "Running processes:\n";
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (!GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            oss << std::left
                << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getName()
                << std::setw(4) << "Core: " << GlobalScheduler::getInstance()->getProcess(i)->getCPUCoreID()
                << std::right << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter()
                << "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

std::string MainConsole::displayFinished() const
{
    std::ostringstream oss;
    oss << "Finished processes:\n";
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            oss << std::left
                << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getName()
                << std::setw(4) << "FINISHED"
                << std::right << std::setw(10) << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter()
                << "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

string MainConsole::getName() const 
{
    return this->name;
}

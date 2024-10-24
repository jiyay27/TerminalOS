#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"

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
            // LAGAY DITO UNG CONFIG CHAKA PAG START NG SCHEDULER
            GlobalScheduler::getInstance()->setCoreCount(4);
			std::cout << GlobalScheduler::getInstance()->getCoreCount() << " cores available." << std::endl;
            cout << "Console has been initialized..." << endl;
            //GlobalScheduler::getInstance()->getScheduler()->init();
			GlobalScheduler::getInstance()->getScheduler()->printCores();

			outputMessage = "";

        }

        if (outputMessage == "clear")
        {
            system("cls");
        }

        if (outputMessage == "exit")
        {
            outputMessage = "";
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
            int cor = GlobalScheduler::getInstance()->getScheduler()->assignCore(GlobalScheduler::getInstance()->getMostRecentProcess());
            std::cout << cor << std::endl;
            GlobalScheduler::getInstance()->getScheduler()->addProcess(GlobalScheduler::getInstance()->getMostRecentProcess(),cor);
        }

        if (outputMessage == "screenls")
        {
            int coreTotal = GlobalScheduler::getInstance()->getCPUWorkers().size();
            int coresUsed = 1;//GlobalScheduler::getInstance()->availableCores();
            this->displayCPUUtil(coresUsed, coreTotal);
            this->displayRunning();
            std::cout << "" << std::endl;
            this->displayFinished();
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
       // schedulertest();  // Assuming schedulertest function is defined
        outputMessage = "Scheduler test started.";
    }
    else if (command == "scheduler-stop") {
      //  schedulerstop();  // Assuming schedulerstop function is defined
        outputMessage = "Scheduler stopped.";
    }
    else if (command == "report-util") {
        //reportutil();  // Assuming reportutil function is defined
        outputMessage = "Utility report generated.";
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

void MainConsole::displayCPUUtil(int coresUsed, int cores) const
{
    int coresAvail = cores - coresUsed;
    float cpuUtil = coresUsed * 100 / cores;
    std::cout << "CPU Utilization: " << cpuUtil << "%" << std::endl;
    std::cout << "Cores used: " << coresUsed << std::endl;
    std::cout << "Cores available: " << coresAvail << std::endl;
    std::cout << "" << std::endl;
}

void MainConsole::displayRunning() const
{
    std::cout << "Running processes:" << std::endl;
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (!GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            std::cout << GlobalScheduler::getInstance()->getProcess(i)->getName() <<
                "   Core: " << GlobalScheduler::getInstance()->getProcess(i)->getCPUCoreID() <<
                "     " << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() <<
                "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << std::endl;
        }
    }
}

void MainConsole::displayFinished() const
{
    std::cout << "Finished processes:" << std::endl;
    for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
    {
        if (GlobalScheduler::getInstance()->getProcess(i)->isFinished())
        {
            std::cout << GlobalScheduler::getInstance()->getProcess(i)->getName() <<
                "   FINISHED " << "     "
                << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() <<
                "/" << GlobalScheduler::getInstance()->getProcess(i)->getCommandListCount() << std::endl;
        }
    }
}

string MainConsole::getName() const 
{
    return this->name;
}

#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"

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
            cout << "Console has been initialized..." << endl;
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
        }

        if (outputMessage == "screenls")
        {
            std::cout << "CPU Utilization: " << "100%" << std::endl;
            std::cout << "Cores used: " << "4" << std::endl;
            std::cout << "Cores available: " << "0" << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Running processes:" << std::endl;
            for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
            {
                if (GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() != GlobalScheduler::getInstance()->getProcess(i)->getTotalInstructions())
                {
                    std::cout << GlobalScheduler::getInstance()->getProcess(i)->getName() <<
                        "   Core: " << GlobalScheduler::getInstance()->getProcess(i)->getCPUCoreID() <<
                        "     " << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() <<
                        "/" << GlobalScheduler::getInstance()->getProcess(i)->getTotalInstructions() << std::endl;
                }
            }

            std::cout << "" << std::endl;


            std::cout << "Finished processes:" << std::endl;
            for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); i++)
            {
                if (GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() == GlobalScheduler::getInstance()->getProcess(i)->getTotalInstructions())
                {
                    std::cout << GlobalScheduler::getInstance()->getProcess(i)->getName() <<
                        "   Core: " << GlobalScheduler::getInstance()->getProcess(i)->getCPUCoreID() <<
                        "     " << GlobalScheduler::getInstance()->getProcess(i)->getCommandCounter() <<
                        "/" << GlobalScheduler::getInstance()->getProcess(i)->getTotalInstructions() << std::endl;
                }
            }

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
    cout << "Enter command: ";
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

string MainConsole::getName() const 
{
    return this->name;
}

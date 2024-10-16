#include "MainConsole.h"
#include "ConsoleManager.h"

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
            ConsoleManager::getInstance()->exitConsole();
        }

        if (outputMessage == "screenr") 
        {

        }

        if (outputMessage == "screens")
        {
            std::shared_ptr<Process> process = std::make_shared<Process>();
            std::shared_ptr<BaseScreen> baseScreen = std::make_shared<BaseScreen>(process,outputArg2);
            ConsoleManager::getInstance()->registerScreen(baseScreen);
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
        // TODO: Handle 'screen -r [name]' command
        outputMessage = "screenr";
        outputArg2 = arg2;
    }
    else if (command == "screen" && arg1 == "-s" && !arg2.empty()) {
        // TODO: Handle 'screen -s [name]' command
        outputMessage = "screens";
        outputArg2 = arg2;
    }
    else if (command == "screen" && arg1 == "-ls") {
        // TODO: Handle 'screen -ls' command (list screens)
        outputMessage = "screenls";
    }
    else {
        outputMessage = "Invalid command...";
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

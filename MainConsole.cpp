#include "MainConsole.h"
#include "ConsoleManager.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
string response = "";
string outputMessage = "";

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

    if (outputMessage.empty()) {
        onEnabled();
    }

    if (outputMessage == "initialize")
    {

    }

    if (outputMessage == "clear")
    {
        system("cls");
    }

    if (outputMessage == "exit")
    {

    }
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
        ConsoleManager::getInstance()->exitConsole();
    }
    else if (command == "initialize") {
        outputMessage = command;
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
        // Handle 'screen -r [name]' command
        outputMessage = "Resuming screen with name: " + arg2;
        // Logic to resume the screen goes here (e.g., checking if screen exists)
    }
    else if (command == "screen" && arg1 == "-ls") {
        // Handle 'screen -ls' command (list screens)
        outputMessage = "Listing all screens.";
        // Logic to list screens goes here
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

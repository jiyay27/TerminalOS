#include "MainConsole.h"
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
string response = "";
string outputMessage = "";
MainConsole::MainConsole(String name) //constructor
{

}

void MainConsole::onEnabled() //main screen start up
{

}

void MainConsole::display() // Displays output
{
    if (!outputMessage.empty()) {
        cout << outputMessage << endl;  // Display the stored message
    }
    else {
        cout << "No command processed yet." << endl;
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
        //clear();  // Clear the console screen (assuming the clear function is defined)
        outputMessage = "Screen cleared.";
    }
    else if (command == "exit") {
        outputMessage = "Exiting the program.";
        return;
    }
    else if (command == "initialize") {
        //initialize();  // Assuming initialize function is defined
        outputMessage = "Initialization complete.";
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

void asciiart() {
    cout << "  _____  _____  ____  _____  ______  _______     __ \n";
    cout << " / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / / \n";
    cout << "| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ /  \n";
    cout << "| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /   \n";
    cout << "| |____ ____) | |__| | |    | |____ ____) |  | |    \n";
    cout << " \\_____|_____/ \\____/|_|    |______|_____/   |_|    \n";
}

void header() {
    SetConsoleTextAttribute(hConsole, 10);
    cout << "Hello, Welcome to CSOPESY commandline!\n";
    SetConsoleTextAttribute(hConsole, 14);
    cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    SetConsoleTextAttribute(hConsole, 15);
}
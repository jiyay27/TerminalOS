#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "ConsoleManager.h"
#include "InputManager.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

void asciiart() {
    cout << "  _____  _____  ____  _____  ______  _______     __ \n";
    cout << " / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / / \n";
    cout << "| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ /  \n";
    cout << "| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /   \n";
    cout << "| |____ ____) | |__| | |    | |____ ____) |  | |    \n";
    cout << " \\_____|_____/ \\____/|_|    |______|_____/   |_|    \n";
}
void header() {
    cout << "Hello, Welcome to CSOPESY commandline!\n";
    cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
}
void clear() {
    system("cls");
    asciiart();
    header();
}

//INITIALIZE COMMAND
void initialize() {
    printf("Initialize command recognized. Doing something...\n");
}
//SCHEDULER TEST COMMAND
void schedulertest() {
    printf("Scheduler-test command recognized. Doing something...\n");
}
//SCHEDULER STOP COMMAND
void schedulerstop() {
    printf("Scheduler-stop command recognized. Doing something...\n");
}
//REPORT UTIL COMMAND
void reportutil() {
    printf("Report-util command recognized. Doing something...\n");
}

void showTimestamp() {
    SYSTEMTIME st;
    GetLocalTime(&st);

    const char* ampm = (st.wHour >= 12) ? "PM" : "AM";

    int hour = st.wHour % 12;
    if (hour == 0) {
        hour = 12;
    }

    printf("(%02d/%02d/%04d, %02d:%02d:%02d %s)",
        st.wMonth,
        st.wDay,
        st.wYear,
        hour,
        st.wMinute,
        st.wSecond,
        ampm);
}

// ! =========================== MAIN =====================================

int main() {
    asciiart();
    header();

    ConsoleManager::initialize();

    bool running = true;
    while (running)
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();

        running = ConsoleManager::getInstance()->isRunning();
    }

    string response = "";

    if (response == "clear") {
        clear();
    }
    else if (response == "exit") {
        return 0;
    }
    else if (response == "initialize") {
        initialize();
    }
    else if (response == "scheduler-test") {
        schedulertest();
    }
    else if (response == "scheduler-stop") {
        schedulerstop();
    }
    else if (response == "report-util") {
        reportutil();
    }
    else {
        cout << "Invalid command..." << endl;
    }

    ConsoleManager::destroy();

    return 0;
}

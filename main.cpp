#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "ConsoleManager.h"
#include "InputManager.h"
#include "GlobalScheduler.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

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
    ConsoleManager::initialize();
	GlobalScheduler::initialize();
    bool running = true;
    while (running)
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();

        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::destroy();
	GlobalScheduler::destroy(); 
    return 0;
}

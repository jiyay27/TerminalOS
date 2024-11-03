#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "ConsoleManager.h"
#include "InputManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

// ! =========================== MAIN =====================================

int main() {
    ConsoleManager::initialize();
    GlobalScheduler::initialize();
    GlobalScheduler::getInstance()->selectScheduler("RR");
    GlobalScheduler::getInstance()->startThreads();
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

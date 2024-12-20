#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"
#include "Config.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// ! =========================== MAIN =====================================

int main() {
    ConsoleManager::initialize();
    GlobalScheduler::initialize();
    FlatMemoryAllocator::initializeMemory();
    PagingAllocator::initializeMemory();

    Config config;
	config.setParamList("config.txt");
	GlobalScheduler::getInstance()->selectScheduler(config.getSchedulerAlg());
	GlobalScheduler::getInstance()->startThreads();
    int mainClock = 0;
    bool running = true;
    while (running)
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();
        running = ConsoleManager::getInstance()->isRunning();
        mainClock++;
    }
	
    ConsoleManager::destroy();
	GlobalScheduler::destroy(); 
    return 0;
}

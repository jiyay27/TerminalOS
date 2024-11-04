#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "Config.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// ! =========================== MAIN =====================================

int main() {
    ConsoleManager::initialize();
    GlobalScheduler::initialize();
    Config config;
	config.setParamList("config.txt");
	GlobalScheduler::getInstance()->selectScheduler(config.getSchedulerAlg());
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

#include "ConsoleManager.h"

#include <iostream>

#include "MainConsole.h"
#include "MarqueeConsole.h"
#include "GlobalScheduler.h"


ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance()
{
    return sharedInstance;
}

void ConsoleManager::initialize()
{
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
    delete sharedInstance;
}

void ConsoleManager::drawConsole() const
{
    if (this->currentConsole != nullptr)
    {
        this->currentConsole->display();
    }
    else
    {
        std::cerr << "There is no assigned console." << std::endl;
    }
}

void ConsoleManager::process() const
{
    if (this->currentConsole != nullptr)
    {
        this->currentConsole->process();
    }
    else
    {
        std::cerr << "There is no assigned console." << std::endl;
    }
}

void ConsoleManager::switchConsole(String consoleName)
{
    if (this->consoleTable.contains(consoleName))
    {
        system("cls");
        this->previousConsole = this->currentConsole;
        this->currentConsole = this->consoleTable[consoleName];
        this->currentConsole->onEnabled();
    }
    else
    {
        std::cerr << "Console name " << consoleName << " not initizalied." << std::endl;
    }
}

void ConsoleManager::returnToPreviousConsole()
{
    if (this->previousConsole != nullptr)
    {
        this->currentConsole = this->previousConsole;
        this->currentConsole->onEnabled();
    }
    else
    {
        std::cerr << "No previous console has been initialized." << std::endl;
    }
}

void ConsoleManager::exitConsole()
{
    this->running = false;
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
    if (this->consoleTable.contains(screenRef->getName()))
    {
        std::cerr << "Screen name " << screenRef->getName() << " already exists." << std::endl;
        return;
    }

    GlobalScheduler::getInstance()->addProcess(screenRef->getProcess());
    this->consoleTable[screenRef->getName()] = screenRef;
    this->switchConsole(screenRef->getName());
}

void ConsoleManager::registerScreen2(std::shared_ptr<BaseScreen> screenRef)
{
    if (this->consoleTable.contains(screenRef->getName()))
    {
        std::cerr << "Screen name " << screenRef->getName() << " already exists." << std::endl;
        return;
    }

    GlobalScheduler::getInstance()->addProcess(screenRef->getProcess());
    this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(String screenName)
{
    if (this->consoleTable.contains(screenName))
    {
        system("cls");
        this->previousConsole = this->currentConsole;
        this->currentConsole = this->consoleTable[screenName];
        this->currentConsole->onEnabled();
    }
    else
    {
        std::cerr << "Screen name " << screenName << " not found." << std::endl;
    }
}

void ConsoleManager::unregisterScreen(String screenName)
{
    if (this->consoleTable.contains(screenName))
    {
        this->consoleTable.erase(screenName);
    }
    else
    {
        std::cerr << "Unable to unregister " << screenName << std::endl;
    }
}

bool ConsoleManager::isRunning() const
{
    return this->running;
}

void ConsoleManager::setCursorPosition(int posX, int posY) const
{
    COORD coord;
    coord = { (SHORT)posX, (SHORT)posX };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleManager::createBaseScreen(String screenName)
{
    std::shared_ptr<Process> process = std::make_shared<Process>(screenName);
    std::shared_ptr<BaseScreen> baseScreen = std::make_shared<BaseScreen>(process, screenName);
    this->registerScreen(baseScreen);
}

void ConsoleManager::createBaseScreen2(String screenName)
{
    std::shared_ptr<Process> process = std::make_shared<Process>(screenName);
    std::shared_ptr<BaseScreen> baseScreen = std::make_shared<BaseScreen>(process, screenName);
    this->registerScreen2(baseScreen);
}

HANDLE ConsoleManager::getConsoleHandle() const
{
    return this->consoleHandle;
}

ConsoleManager::ConsoleManager()
{
    this->running = true;

    this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
    //const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
    //const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
    //const std::shared_ptr<MemoryConsole> memoryConsole = std::make_shared<MemoryConsole>();

    this->consoleTable[MAIN_CONSOLE] = mainConsole;
    //this->consoleTable[MARQUEE_CONSOLE] = marqueeConsole;
    //this->consoleTable[SCHEDULING_CONSOLE] = schedulingConsole;
    //this->consoleTable[MEMORY_CONSOLE] = memoryConsole;

    this->switchConsole(MAIN_CONSOLE);
}

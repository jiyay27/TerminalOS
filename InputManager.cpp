#include "InputManager.h"

String InputManager::getInputPrompt(const String prompt)
{
	String input;
	std::cout << prompt;
	std::getline(std::cin, input);
	return input;
}

void InputManager::clear()
{
    system("cls");
    asciiart();
    header();
}

void InputManager::header()
{
    //green();
    std::cout << "Hello, Welcome to CSOPESY commandline!\n";
    //yellow();
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    //white();
}

void InputManager::asciiart()
{
    std::cout << "  _____  _____  ____  _____  ______  _______     __ \n";
    std::cout << " / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / / \n";
    std::cout << "| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ /  \n";
    std::cout << "| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /   \n";
    std::cout << "| |____ ____) | |__| | |    | |____ ____) |  | |    \n";
    std::cout << " \\_____|_____/ \\____/|_|    |______|_____/   |_|    \n";
}

static std::map<String, String> parseCommandWithArgs(const String& input) {
    std::istringstream stream(input);
    std::map<String, String> parsedCommand;
    String command;
    String flag;
    String value;

    stream >> command;  // Extract the command
    parsedCommand["command"] = command;

    // Extract flags and their associated values (if any)
    while (stream >> flag) {
        if (flag[0] == '-') {
            if (stream >> value && value[0] != '-') {
                parsedCommand[flag] = value;  // Assign flag to value
            }
            else {
                parsedCommand[flag] = "";  // Flag without a value
            }
        }
    }

    return parsedCommand;
}

void InputManager::getCommand(String command)
{
    std::map<String, String> parsedCommand = parseCommandWithArgs(command);

    if (command == "clear") 
    {
        clear();
    }
    else if (command == "exit") 
    {

    }
    else if (command == "marquee") 
    {
        
    }
    else if (parsedCommand["command"] == "screen")
    {
        for (const auto& [flag, value] : parsedCommand) {
            if (flag != "command") {
                std::cout << "Flag: " << flag << " | Value: " << value << std::endl;
            }
        }
    }
    else if (command == "process-smi") 
    {
       
    }
    else if (command == "nvidia-smi") 
    {
        
    }
    else 
    {
        std::cout << "Invalid command..." << std::endl;
    }
}
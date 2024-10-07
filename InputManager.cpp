#include "InputManager.h"
#include <iostream>

String InputManager::getInputPrompt(const String prompt)
{
	String input;
	std::cout << prompt;
	std::getline(std::cin, input);
	return input;
}
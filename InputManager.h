#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

typedef std::string String;

class InputManager
{
public:
    static String getInputPrompt(const String prompt);
    static void getCommand(String command);

    static void clear();
    static void header();
    static void asciiart();

    static std::map<String, String> parseCommandWithArgs(const String& input)
};
#pragma once

#include <string>

typedef std::string String;

class InputManager
{
public:
    typedef std::string String;
    
    static String getInputPrompt(const String prompt);
};
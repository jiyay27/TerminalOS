#pragma once
#include <string>

class AConsole
{
public:
    typedef std::string String;
    AConsole(String name = "placeholder");
    ~AConsole() = default;

    String getName() const;
    virtual void onEnabled() = 0;
    virtual void display() = 0;
    virtual void process() = 0;

    String name;
    friend class ConsoleManager;
};


#pragma once

#include <string>
#include <memory>

typedef std::string String;

class Process
{
public:
	Process();
	Process(String processName);

	int getID();
	String getProcessName();
	int getCurrentLine();
	
private:
	int pid = 0;
	String processName = "";
	int currentLine = 0;
};
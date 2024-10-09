#pragma once

#include <string>
#include <memory>

typedef std::string String;

class Process
{
public:
	Process(String processName);

	int getID() const;
	String getProcessName() const;
	int getCurrentLine() const;

	int pid = 0;
	String processName;
	int currentLine;
};
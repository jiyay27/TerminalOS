#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include "AConsole.h"

using namespace std;

class MainConsole : public AConsole
{
public:
	MainConsole();
	MainConsole(String name);
	~MainConsole() = default;

	String getName() const;
	void onEnabled() override;
	void display() override;
	void process() override;

	void header() const;
	void asciiart() const;

	void displayCPUUtil(int coresUsed, int cores) const;
	void displayRunning() const;
	void displayFinished() const;

	String name;
	friend class ConsoleManager;
};
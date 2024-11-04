#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include "AConsole.h"
#include "Config.h"

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

	std::string displayCPUUtil(int coresUsed, int cores) const;
	std::string displayRunning() const;
	std::string displayFinished() const;


	Config config;
	String fileName = "config.txt";
	String name;
	friend class ConsoleManager;
};
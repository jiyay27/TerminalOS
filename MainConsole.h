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

	// display functions
	String displayCPUUtil() const;
	String displayRunning();
	String displayFinished();
	String displayProcessSMI();

	// compute functions for display
	float computeCoreUtil() const;
	float computeMemoryUtil() const;

	int computeCoresUsed() const;
	int computeCoresAvail() const;

	int computeMemoryUsed() const;
	int computeMemoryAvail() const;

	// helper functions for printing
	String truncateLeftLine(String str, int maxLength);
	String truncateRightLine(String str, int maxLength);

	Config config;
	String fileName = "config.txt";
	String name;
	friend class ConsoleManager;
};
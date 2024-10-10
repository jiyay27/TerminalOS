#pragma once
#include <string>
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);
	~MainConsole() = default;

	String getName() const;
	void onEnabled() override;
	void display() override;
	void process() override;


private:
	String name;
};
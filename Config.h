#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include "TypedefRepo.h"

class Config 
{

public:
    std::vector<std::string> readFileToVector(const std::string& fileName) {
        std::vector<std::string> lines;
        std::ifstream file(fileName);  // Open file

        if (!file) {  // Check if file opened successfully
            std::cerr << "Error: Could not open the file " << fileName << std::endl;
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {  // Read line by line
            std::istringstream iss(line);
            std::string varName, varValue;
            if (iss >> varName >> varValue) {
                lines.push_back(varValue);  // Add only the value to the vector
            }
        }

        file.close();  // Close the file
        return lines;
    }

    void setParamList(const std::string& fileName) {
        this->paramList = readFileToVector(fileName);
        
        for (int i = 0; i < paramList.size(); i++) {
            assignParameter(i);
        }
    }

    void assignParameter(int index) {
        try {
            if (index == 1) { // no need conversion
                this->schedulerAlg = paramList[1];
            }
            else if (index == 0) {
                this->numCPU = std::stoi(paramList[0]);
            }
            else if (index == 2) {
                this->quantum = std::stoi(paramList[index]);
            }
            else if (index == 3) {
                this->bpFrequency = std::stoi(paramList[index]);
            }
            else if (index == 4) {
                this->minIns = std::stoi(paramList[index]);
            }
            else if (index == 5) {
                this->maxIns = std::stoi(paramList[index]);
            }
            else if (index == 6) {
                this->delays = std::stoi(paramList[index]);
            }
            else if (index == 7) {
                this->maxMem = std::stoi(paramList[index]);
            }
            else if (index == 8) {
                this->memFrame = std::stoi(paramList[index]);
            }
            else if (index == 9) {
                this->maxMemProc = std::stoi(paramList[index]);
            }
            else if (index == 10) {
                this->minMemProc = std::stoi(paramList[index]);
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << paramList[index] << " at index " << index << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << paramList[index] << " at index " << index << std::endl;
        }
    }

    int getNumCPU (){
        return this->numCPU;
    }

    int getQuantum() {
        return this->quantum;
    }

    String getSchedulerAlg() {
        return this->schedulerAlg;
    }

    int getBpFrequency() {
		return this->bpFrequency;
    }

	int getMinIns() {
		return this->minIns;
	}

	int getMaxIns() {
		return this->maxIns;
	}

	int getDelays() {
		return this->delays;
	}

    // ! MO2 PARAMS
    int getMaxMem() {
        return this->maxMem;
    }

    int getMemFrame() {
        return this->memFrame;
    }

    int getMinMemProc() {
        return this->minMemProc;
    }

    int getMaxMemProc() {
        return this->maxMemProc;
    }

    std::string removeQuotes(const std::string& str) {
        if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
            return str.substr(1, str.size() - 2);
        }
        return str;
    }
private:
    std::vector<std::string> paramList;

	// ! MO1 PARAMS
	int numCPU; // 0
    String schedulerAlg; // 1
	int quantum; // 2
	int bpFrequency; // 3
	int minIns; // 4
	int maxIns; // 5
	int delays; // 6

	// ! MO2 PARAMS
	size_t maxMem; // 7
    size_t memFrame; // 8
    size_t minMemProc; // 9
    size_t maxMemProc; // 10

};
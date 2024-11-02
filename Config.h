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
            lines.push_back(line);  // Add each line to the vector
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
        if (index == 2) {// no need conversion
            this->schedulerAlg = paramList[2];
        }
        else if(index == 0){
            this->numCPU = std::stoi(paramList[0]);
        }
        else if (index == 1) {
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
    }
private:
    std::vector<std::string> paramList;
	int numCPU; // 0
	int quantum; // 1
	String schedulerAlg; // 2
	int bpFrequency; // 3
	int minIns; // 4
	int maxIns; // 5
	int delays; // 6

};
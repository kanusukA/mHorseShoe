#pragma once

#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

enum CMDST{
	eADD,
	eTEST,
	eREMOVE
};

enum PHYST {
	eDYNAM,
	eSTATIC
};

struct BCMDS {

	CMDST cmdt;
	PHYST physt;

	string name;
	string mshName;

	vector<int>* pos;

};



bool runCommand(std::string cmd, BCMDS* output);

bool _AddCommand(BCMDS* output);


vector<int>* _getposition(string pos);



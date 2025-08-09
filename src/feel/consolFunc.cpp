#include "consolFunc.h"

// OLD SHIT , DONT BOTHER

bool runCommand(std::string cmd, BCMDS* output) {

	
	//ADD
	if (cmd == "ADD") {
		return _AddCommand(output);
	}
	else if (cmd == "TEST")
	{
		output->cmdt = CMDST::eTEST;
	}
	else if (cmd == "REMOVE") {

	}

	return true;

}

bool _AddCommand(BCMDS* output) {

	vector<string> strVec;
	
	string name , meshName, physXType, pos;
	cout << "\nAdd Stuff : Name MeshName (R)Rigid/(S)Static position(0,0,0)" << endl;

	cin >> name >> meshName >> physXType >> pos;

	string temp = " ";
	
	//null check
	if (name == "" || meshName == "" || physXType == "" || pos == "") {
		cout << "\nInvalid Parameters";
		return false;
	}
	else {
		vector<int>* position = _getposition(pos);
		if (position->size() != 3) {
			cout << "\nInvalid Parameters";
			return false;
		}
		else {

			output->cmdt = CMDST::eADD;

			if (physXType == "r" or physXType == "R") {
				output->physt = PHYST::eDYNAM;
			}
			else if (physXType == "s" or physXType == "S")
			{
				output->physt = PHYST::eSTATIC;
			}
			else {
				cout << "\nInvalid Parameters";
				return false;
			}

			output->name = name;
			output->mshName = meshName;
			output->pos = position;


		}
		
	}    

	return true;

}

vector<int>* _getposition(string pos)
{
	cout << "\nposition : " << pos << endl;
	cout << stoi(pos.substr(0, pos.find(',')));

	vector<int>* position = new vector<int>();
	position->push_back(stoi(pos.substr(0, pos.find(','))));

	pos = pos.substr(pos.find(',') + 1);

	position->push_back(stoi(pos.substr(0, pos.find(','))));
	position->push_back(stoi(pos.substr(pos.find(',') + 1)));

	return position;

}






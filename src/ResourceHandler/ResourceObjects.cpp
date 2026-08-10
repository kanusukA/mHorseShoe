#include "ResourceObjects.h"

bool vectorPathContains(std::vector<std::filesystem::path>* checkVec, std::string file)
{
	for (int i = 0; i < checkVec->size(); i++)
	{
		if (checkVec->at(i).filename().string() == file)
		{
			return true;
		}
	}
	return false;
}

// TOOLS


std::string convertFloatPtrToString(float* val_p, int size)
{
	std::string val = std::string("");
	for (int i = 0; i < size; i++)
	{
		val += std::to_string(val_p[i]) + "|";
	}
	return val;
}

int convertStringToFloatPtr(std::string str_p, float* &output_p)
{
	std::vector<std::string>* vecOfValues = new std::vector<std::string>();
	std::string temp = std::string();
	for (int i = 0; i < str_p.size(); i++)
	{
		if (str_p.at(i) != '|')
		{
			temp += str_p.at(i);
		}
		else {
			vecOfValues->push_back(temp);
			temp = "";
		}
	}

	output_p = new float[vecOfValues->size()]();
	
	for (int i = 0; i < vecOfValues->size(); i++)
	{
		output_p[i] = std::stof(vecOfValues->at(i));
		std::cout << output_p[i] << std::endl;
	}

	int size = vecOfValues->size();

	delete vecOfValues;

	return size;



}


std::string convertShaderVarValueToStr(ShaderVar variable) {
	switch (variable.varType)
	{
	case ShaderVarType::FLOAT0:
		return std::to_string(*variable.varFloat);
		break;
	case ShaderVarType::FLOAT2:
		return std::to_string(variable.varFloat2[0]) + "|" + std::to_string(variable.varFloat2[0]) + "|";
		break;
	case ShaderVarType::FLOAT3:
		return std::to_string(variable.varFloat3[0]) + "|" + std::to_string(variable.varFloat3[0]) + "|" + std::to_string(variable.varFloat3[2]) + "|";
		break;
	case ShaderVarType::FLOAT4:
		return std::to_string(variable.varFloat4[0]) + "|" + std::to_string(variable.varFloat4[0]) + "|" + std::to_string(variable.varFloat4[2]) + "|" + std::to_string(variable.varFloat4[3]) + "|";
		break;
	case ShaderVarType::INTEGER:
		return std::to_string(*variable.varInt);
		break;
	default:
		// TODO Add resource handler Error
		break;
	}
}
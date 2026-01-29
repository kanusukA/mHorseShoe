#include "ResourceObjects.h"

// TOOLS

std::string convertVec3ToString(Ogre::Vector3 vec3) {
	return std::to_string(vec3[0]) + "|" + std::to_string(vec3[1]) + "|" + std::to_string(vec3[2]) + "|";
}

Ogre::Vector3 convertStringToVec3(std::string str) {
	std::string value = "";
	int count = 0;
	Ogre::Vector3 vec3 = Ogre::Vector3();
	for (int i = 0; i < str.size(); i++)
	{
		if (str.at(i) == '|')
		{
			vec3[count] = std::stof(value);
			value.clear();
			count += 1;
		}
		else {
			value += str.at(i);
		}
	}
	return vec3;
}

std::string convertVec4ToString(Ogre::Vector4 vec4) {
	return std::to_string(vec4[0]) + "|" + std::to_string(vec4[1]) + "|" + std::to_string(vec4[2]);
}

Ogre::Vector4 convertStringToVec4(std::string str) {
	std::string value = "";
	int count = 0;
	Ogre::Vector4 vec4 = Ogre::Vector4();
	for (int i = 0; i < str.size(); i++)
	{
		if (str.at(i) == '|')
		{
			vec4[count] = std::stof(value);
			value.clear();
			count += 1;
		}
		else {
			value += str.at(i);
		}
	}
	return vec4;
}

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

Ogre::Vector3 convertFloatPtrToVec3(float* val_p)
{
	return Ogre::Vector3(val_p[0], val_p[1], val_p[2]);
}

Ogre::Vector4 convertFloatPtrToVec4(float* val_p)
{
	return Ogre::Vector4(val_p[0], val_p[1], val_p[2],val_p[3]);
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
#pragma once

#include <Gui/GuiComponents/ToastComponent.h>

#include<Windows.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <filesystem>


enum ShaderVarType
{
	INTEGER,
	FLOAT0,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	RBOOL
};

struct ShaderVar {

	std::string varName;
	ShaderVarType varType;

	int* varInt = new int(0);
	float* varFloat = new float(0.0);
	float varFloat2[2] = { 0.0,0.0 };
	float varFloat3[3] = { 0.0, 0.0, 0.0 };
	float varFloat4[4] = { 0.0, 0.0, 0.0, 0.0 };

};

enum ShaderType {
	Vertex,
	Fragment
};

class ResourceReader {

private:

	std::ifstream inStream;

public:

	std::string cleanWord(std::string word);

	// MATERIAL FUNCS
	std::string readMaterialName(std::filesystem::path mat_path_p);

	void readShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p);
	void readGLSLShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p);


};
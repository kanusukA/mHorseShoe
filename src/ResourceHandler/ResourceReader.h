#pragma once

#include <Gui/GuiComponents/ToastComponent.h>


#include<Windows.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

// DO NOT CHANGE THE ORDER!!!!!!!
enum ShaderVarType
{
	BLANK,
	INTEGER,
	FLOAT0,
	FLOAT2,
	FLOAT3,
	FLOAT4,
};

struct ShaderVar {

	std::string varName;
	ShaderVarType varType;

	int* varInt = new int(0);
	float* varFloat = new float(0.0);
	/*float varFloat2[2] = { 0.0,0.0 };
	float varFloat3[3] = { 0.0, 0.0, 0.0 };
	float varFloat4[4] = { 0.0, 0.0, 0.0, 0.0 };*/

	glm::vec2 varFloat2 = glm::vec2(0.0f);
	glm::vec3 varFloat3 = glm::vec3(0.0f);
	glm::vec4 varFloat4 = glm::vec4(0.0f);

	int padding; // to align with memory layout

};



enum ShaderType {
	Vertex,
	Fragment
};

class ResourceReader {

private:

	std::ifstream inStream;

public:

	std::string cleanWord(std::string word, bool containsDigits = true);

	// MATERIAL FUNCS
	std::string readMaterialName(std::filesystem::path mat_path_p);

	void readShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p);
	void readGLSLShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p);

	const std::string& readFileContents(const std::filesystem::path& filePath);
	const std::vector<char>& readFileContentsChar(const std::filesystem::path& filePath);
	void readFileContents(const std::filesystem::path& filePath, std::vector<char>* outputVector);


};
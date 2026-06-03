#include "ResourceReader.h"

std::string ResourceReader::cleanWord(std::string word, bool containss)
{
	std::string output = "";
	for (int i = 0; i < word.size(); i++)
	{
		if ((word.at(i) < 91 && word.at(i) > 64) || (word.at(i) < 123 && word.at(i) > 96) || (containss && word.at(i) >= 48 && word.at(i) <= 57))
		{
			output.push_back(word.at(i));
		}
	}
	return output;
}

std::string ResourceReader::readMaterialName(std::filesystem::path mat_path_p)
{
    inStream.open(mat_path_p);

	if (inStream.is_open())
	{

		std::string word;

		bool materialFound = false;

		while (inStream >> word) {
			if (materialFound)
			{
				inStream.close();
				return cleanWord(word); // fix this
			}
			else {
				if (word == "material")
				{
					materialFound = true;
				}
			}
		}

	}
	else {
		ToastComponent::GetInstance()->addMessage("Failed to open inStream file");
	}

	inStream.close();

}

// HLSL SPECIFIC!
void ResourceReader::readShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p)
{
	
	if (inStream.is_open())
	{
		inStream.close();
	}


	inStream.open(shaderPath_p);
	std::string line;

	std::string word = "";

	bool uniFound = false;
	bool uniName = false;
	bool skip = false;
	int type;

	ShaderVar shaderVar = ShaderVar();

	if (inStream.is_open()) {
		while (std::getline(inStream, line))
		{
			//std::cout << line << std::endl;
			if (line == "//SKIP")
			{
				skip = true;
			}

			//std::cout << "Line : " << line << std::endl;

			if (skip)
			{
				if (line == "//!SKIP") {
					skip = false;
					//shaderVar->push_back(word);
				}

				continue;
			}

			// first check uniform keyword
			for (int i = 0; i < line.size(); i++)
			{


				if (line.at(i) != ' ' && line.at(i) != ',' && line.at(i) != ')') {
					word += line.at(i);
				}

				else {
					// skips the coming constants

					if (word == "uniform") {

						//std::cout << "uniform" << std::endl;
						uniFound = true;
						shaderVar = ShaderVar();


					}
					else if (uniFound) {
						//std::cout << "type : ";

						uniFound = false;
						uniName = true;

						if (word == "int" || word == "bool") {
							shaderVar.varType = ShaderVarType::INTEGER;
							*shaderVar.varInt = 0;
						}
						else if (word == "float")
						{
							shaderVar.varType = ShaderVarType::FLOAT0;
							shaderVar.varFloat = new float(0.0);
						}
						else if (word == "float2")
						{
							shaderVar.varType = ShaderVarType::FLOAT2;
							shaderVar.varFloat2[0] = 0.0f;
							shaderVar.varFloat2[1] = 0.0f;
						}
						else if (word == "float3")
						{
							shaderVar.varType = ShaderVarType::FLOAT3;
							shaderVar.varFloat3[0] = 0.0f;
							shaderVar.varFloat3[1] = 0.0f;
							shaderVar.varFloat3[2] = 0.0f;
						}
						else if (word == "float4")
						{
							shaderVar.varType = ShaderVarType::FLOAT4;
							shaderVar.varFloat4[0] = 0.0f;
							shaderVar.varFloat4[1] = 0.0f;
							shaderVar.varFloat4[2] = 0.0f;
							shaderVar.varFloat4[3] = 0.0f;
						}
						else {
							std::cout << "invalid type : " << uniName << " , " << word << std::endl;
							uniName = false;
						}


					}
					else if (uniName)
					{
						shaderVar.varName = word;
						output_p->push_back(shaderVar);
						uniName = false;
					}
					word = "";
				}
			}

		}
	}

	inStream.close();


}

void ResourceReader::readGLSLShaderFile(std::filesystem::path shaderPath_p, std::vector<ShaderVar>* output_p)
{

	if (inStream.is_open())
	{
		inStream.close();
	}


	inStream.open(shaderPath_p);
	std::string line;

	std::string word = "";

	bool OgreUniformsFound = false;
	int type;

	ShaderVar shaderVar = ShaderVar();

	if (inStream.is_open()) {
		while (std::getline(inStream, line))
		{
			//std::cout << line << std::endl;
			if (!OgreUniformsFound && line.find("OgreUniforms") != std::string::npos)
			{
				OgreUniformsFound = true;
				continue;
			}

			if (OgreUniformsFound && line.find("}") != std::string::npos)
			{
				break;
			}

			if (OgreUniformsFound)
			{
				// ltrim
				line.erase(line.begin(), std::ranges::find_if_not(line, [](unsigned char c) {
					return std::isspace(c);
				}));
				// rtrim
				line.erase(std::find_if_not(line.rbegin(), line.rend(), [](unsigned char c) {
					return std::isspace(c);
					}).base(), line.end());

				if (line.length() == 0 || line.at(0) == '/') 
				{
					//comment line hit.
					continue;
				}

				std::string type = line.substr(0, line.find_first_of(' '));

				std::string name = line.substr(line.find_first_of(' ') + 1, line.find_first_of(';') - line.find_first_of(' ') - 1);

				std::cout << "Name : " << name << " Type : " << type << std::endl;

				if (type == "int")
				{
					shaderVar.varType = ShaderVarType::INTEGER;
					*shaderVar.varInt = 0;
					
				}
				else if (type == "float")
				{
					shaderVar.varType = ShaderVarType::FLOAT0;
					shaderVar.varFloat = new float(0.0);
				}
				else if (type == "vec2")
				{
					shaderVar.varType = ShaderVarType::FLOAT2;
					shaderVar.varFloat2[0] = 0.0f;
					shaderVar.varFloat2[1] = 0.0f;
				}
				else if (type == "vec3")
				{
					shaderVar.varType = ShaderVarType::FLOAT3;
					shaderVar.varFloat3[0] = 0.0f;
					shaderVar.varFloat3[1] = 0.0f;
					shaderVar.varFloat3[2] = 0.0f;
				}
				else if (type == "vec4")
				{
					shaderVar.varType = ShaderVarType::FLOAT4;
					shaderVar.varFloat4[0] = 0.0f;
					shaderVar.varFloat4[1] = 0.0f;
					shaderVar.varFloat4[2] = 0.0f;
					shaderVar.varFloat4[3] = 0.0f;
				}
				else {
					ToastComponent::GetInstance()->addMessage("invalid type : " + type + " in shader file : " + shaderPath_p.string());
					continue;
				}

				shaderVar.varName = name;
				output_p->push_back(shaderVar);
				shaderVar = ShaderVar();
				
			}
			
		}
	}

	inStream.close();


}

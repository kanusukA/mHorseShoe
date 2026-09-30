#pragma once

#include <GDHandler/GDContext.h>


struct test {
	float o;
	float b;
	alignas(16)
	glm::vec3 oki;

};

class Shader : public ShaderResource {
private:
	
	GDBuilderContext* GDBuilderCxt;

	// READS AND SETS SHADER VARS FROM MATERIAL FILE
	void _setShaderVars() {};

public:

	Shader(GDBuilderContext* GDBuilderCxt_p,std::string name_p,ShaderType shaderType ,std::filesystem::path vertPath, std::filesystem::path fragPath) : 
		ShaderResource(ResourceHandler::GetInstance(), name_p, shaderType, vertPath, fragPath)
	{
		GDBuilderCxt = GDBuilderCxt_p;

		_setShaderVars();
		loadShaderVar();

	}

	void loadShaderVar() 
	{
		//ResourceHandler::GetInstance()->readGLSLShaderFile(vertPath, vertShaderParameters); // Unifrom buffers are not yet read as they might collide with the transformation buffer
		ResourceHandler::GetInstance()->readGLSLShaderFile(fragPath, fragShaderParameters);
		alignShaderVar();
		calShaderDeviceSizes();
	}

	void alignShaderVar() {
		// "largest" in this function refers to the byte size of variables NOT their containts
		
		// To align shaerVars in vector to that of shader variables in gpu, we first find if the largest element is 16 or 8 (12 is treated as 16), each element is added until 16 is the sum,
		// if 16 is the exact sum it resets and loop continus and 
		// if at any point the sum exceeds 16, then the difference is padded to the previous element and the alignment continus

		if (fragShaderParameters->empty() || fragShaderParameters->size() == 1){
			return; // No Alignment needed
		}
		

		// The largest element will always be at the end of the vector, due to the way it's defined in the shader code.
		// IF BY CHANCE THIS RULE IS NOT FOLLOWED, AMEND IS TO BE MADE IN THE SHADER NOT IN THIS FUNCTION!

		uint32_t sum = 0;

		// align to 8 bytes
		if (fragShaderParameters->back().varType == ShaderVarType::FLOAT2)
		{
			for (size_t i = 0; i < fragShaderParameters->size(); i++)
			{
				ShaderVarType type = fragShaderParameters->at(i).varType;
				if (type < ShaderVarType::FLOAT2)
				{
					sum += 4;
				}
				else if(type == ShaderVarType::FLOAT2)
				{
					sum += 8;
				}

				if (sum == 8)
				{
					sum = 0;
				}
				else if (sum > 8)
				{
					fragShaderParameters->at(i - 1).padding = sum - 8;
					sum = 0;
				}
				
			}
		}

		// align to 16 bytes
		else if (fragShaderParameters->back().varType == ShaderVarType::FLOAT3 || fragShaderParameters->back().varType == ShaderVarType::FLOAT4)
		{
			for (size_t i = 0; i < fragShaderParameters->size(); i++)
			{
				ShaderVarType type = fragShaderParameters->at(i).varType;
				
				if (type == ShaderVarType::FLOAT3)
				{
					fragShaderParameters->at(i).padding += 4; // padding is added to align 12 to 16
				}

				if (type < ShaderVarType::FLOAT2)
				{
					sum += 4;
					fragShaderSize += 4;
					
				}
				else if (type == ShaderVarType::FLOAT2)
				{
					sum += 8;
					if (sum > 16)
					{
						fragShaderParameters->at(i - 1).padding =  16 - (sum - 8);
						sum = 0;
					}
				}
				else if (type <= ShaderVarType::FLOAT4)
				{
					sum += 16;
					if (sum > 16)
					{
						fragShaderParameters->at(i - 1).padding = 16 - (sum - 16);
						sum = 0;
					}
				}

				if (sum == 16)
				{
					sum = 0;
				}
				else if (sum > 16)
				{
					throw std::runtime_error("MISSED ALIGNMENT!");
				}
				

			}
		}

	}

	void calShaderDeviceSizes() {
		for (const auto& var : *fragShaderParameters)
		{
			switch (var.varType) {
			case ShaderVarType::FLOAT0:
				fragShaderSize += 4;
				break;
			case ShaderVarType::FLOAT2:
				fragShaderSize += 8;
				break;
			case ShaderVarType::FLOAT3:
				fragShaderSize += 12;
				break;
			case ShaderVarType::FLOAT4:
				fragShaderSize += 16;
				break;
			default:
				throw std::runtime_error("INVALID VAR");
			}

			fragShaderSize += var.padding;

		}
		std::cout << "Frag Size : " << fragShaderSize << std::endl;
	}

	void loadShader();

	

	// Runs and updates shader values with Rsus
	void _refreshShader();





};
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
	
	

	// READS AND SETS SHADER VARS FROM MATERIAL FILE
	void _setShaderVars() {};

public:

	GDBuilderContext* GDBuilderCxt;

	Shader(GDBuilderContext* GDBuilderCxt_p,std::string name_p,ShaderType shaderType ,std::filesystem::path vertPath, std::filesystem::path fragPath) : 
		ShaderResource(ResourceHandler::GetInstance(), name_p, shaderType, vertPath, fragPath)
	{
		GDBuilderCxt = GDBuilderCxt_p;

		_setShaderVars();
		loadShaderVar();
		loadPushConst();

	}

	void loadShaderVar() 
	{
		ResourceHandler::GetInstance()->readGLSLShaderFile(vertPath, &vertShaderParameters); 
		ResourceHandler::GetInstance()->readGLSLShaderFile(fragPath, &fragShaderParameters);
		enableFrag = !fragShaderParameters.empty();
		enableVert = !vertShaderParameters.empty();
		alignShaderVar(fragShaderParameters);
		alignShaderVar(vertShaderParameters);
		fragShaderSize = calShaderDeviceSizes(fragShaderParameters);
		vertexShaderSize = calShaderDeviceSizes(vertShaderParameters);
	}

	// TODO create a better push Const system
	void loadPushConst() {
		ResourceHandler::GetInstance()->readGLSLShaderFile(vertPath, &vertPushConsts,"PushConstants");
		ResourceHandler::GetInstance()->readGLSLShaderFile(fragPath, &fragPushConsts,"PushConstants");
		pushConstSize = calShaderDeviceSizes(fragPushConsts);
	}


	// NOTHING BELOW THIS SHOULD BE DISTUREBED! --------------------------------------------------------------------------------------------------------------------------------------

	void alignShaderVar(std::vector<ShaderVar>& params) {
		// "largest" in this function refers to the byte size of variables NOT their containts
		
		// To align shaerVars in vector to that of shader variables in gpu, we first find if the largest element is 16 or 8 (12 is treated as 16), each element is added until 16 is the sum,
		// if 16 is the exact sum it resets and loop continus and 
		// if at any point the sum exceeds 16, then the difference is padded to the previous element and the alignment continus

		if (params.empty() || params.size() == 1){
			return; // No Alignment needed
		}

		// The largest element will always be at the end of the vector, due to the way it's defined in the shader code.
		// IF BY CHANCE THIS RULE IS NOT FOLLOWED, AMEND IS TO BE MADE IN THE SHADER NOT IN THIS FUNCTION!

		uint32_t sum = 0;

		// align to 8 bytes
		if (params.back().varType == ShaderVarType::FLOAT2)
		{
			for (size_t i = 0; i < params.size(); i++)
			{
				ShaderVarType type = params.at(i).varType;
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
					params.at(i - 1).padding = sum - 8;
					sum = 0;
				}
				
			}
		}

		// align to 16 bytes
		else if (params.back().varType == ShaderVarType::FLOAT3 || params.back().varType == ShaderVarType::FLOAT4)
		{
			for (size_t i = 0; i < params.size(); i++)
			{
				ShaderVarType type = params.at(i).varType;
				
				if (type == ShaderVarType::FLOAT3)
				{
					params.at(i).padding += 4; // padding is added to align 12 to 16
				}

				if (type < ShaderVarType::FLOAT2)
				{
					sum += 4;
					
				}
				else if (type == ShaderVarType::FLOAT2)
				{
					sum += 8;
					if (sum > 16)
					{
						params.at(i - 1).padding =  16 - (sum - 8);
						sum = 0;
					}
				}
				else if (type <= ShaderVarType::FLOAT4)
				{
					sum += 16;
					if (sum > 16)
					{
						params.at(i - 1).padding = 16 - (sum - 16);
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

	vk::DeviceSize calShaderDeviceSizes(std::vector<ShaderVar>& params) {
		vk::DeviceSize size = 0;
		for (const auto& var : params)
		{
			switch (var.varType) {
			case ShaderVarType::FLOAT0:
				size += 4;
				break;
			case ShaderVarType::FLOAT2:
				size += 8;
				break;
			case ShaderVarType::FLOAT3:
				size += 12;
				break;
			case ShaderVarType::FLOAT4:
				size += 16;
				break;
			default:
				throw std::runtime_error("INVALID VAR");
			}

			size += var.padding;

		}
		std::cout << "Frag Size : " << size << std::endl;
		return size;
	}

	void loadShader();

	

	// Runs and updates shader values with Rsus
	void _refreshShader();





};
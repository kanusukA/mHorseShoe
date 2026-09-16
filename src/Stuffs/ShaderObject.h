#pragma once

#include <GDHandler/GDContext.h>

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

	}

	void loadShaderVar(std::vector<ShaderVar> vars_p) {};

	void loadShader();

	

	// Runs and updates shader values with Rsus
	void _refreshShader();





};
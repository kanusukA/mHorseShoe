#pragma once

#include <GDHandler/GDContext.h>

class Shader : public ShaderResource {
private:
	
	Ogre::GpuProgramParametersPtr shaderParams;
	Ogre::GpuProgramPtr shaderProgram;
	GDBuilderContext* GDBuilderCxt;

	// Sets Up shader type and assigns ShaderVars for this shader
	void _initShader(Ogre::MaterialPtr mat_p);

	// READS AND SETS SHADER VARS FROM MATERIAL FILE
	void _setShaderVars();

public:

	Shader(GDBuilderContext* GDBuilderCxt_p, Ogre::MaterialPtr mat, ShaderType shaderType_p) : 
		ShaderResource(ResourceHandler::GetInstance(), "", shaderType_p, "")
	{
		GDBuilderCxt = GDBuilderCxt_p;

		_initShader(mat);
		_setShaderVars();

	}

	void loadShaderVar(std::vector<ShaderVar> vars_p);

	void loadShader();

	Ogre::GpuProgramParametersPtr getShader() { return shaderParams; }

	// Runs and updates shader values with Rsus
	void _refreshShader();





};
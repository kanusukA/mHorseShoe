#pragma once

#include <GDHandler/GDContext.h>

class Shader : public ShaderResource {
private:
	
	Ogre::GpuProgramParametersPtr shader;
	GDBuilderContext* GDBuilderCxt;

	// Sets Up shader type and assigns ShaderVars for this shader
	void _initShader(Ogre::MaterialPtr mat_p);

	// used to read shaderVar from file
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

	Ogre::GpuProgramParametersPtr getShader() { return shader; }

	// Runs and updates shader values with Rsus
	void _refreshShader();





};
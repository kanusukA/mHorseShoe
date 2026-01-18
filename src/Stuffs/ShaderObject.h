#pragma once

#include <GDHandler/GDContext.h>

class Shader : public ShaderResource {
private:
	
	Ogre::GpuProgramParametersPtr shader;
	GDBuilderContext* builderCxt;

	// Sets Up shader type and assigns ShaderVars for this shader
	void _initShader(Ogre::MaterialPtr mat_p);
	void _setShaderVars();

public:

	Shader(GDBuilderContext* builderCxt_p, Ogre::MaterialPtr mat, ShaderType shaderType_p) : 
		ShaderResource(ResourceHandler::GetInstance(), "", shaderType_p, "")
	{
		builderCxt = builderCxt_p;

		_initShader(mat);
		_setShaderVars();

	}



	void loadShader();

	Ogre::GpuProgramParametersPtr getShader() { return shader; }



};
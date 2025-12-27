#pragma once

#include <GDHandler/GDContext.h>

class Shader : public ShaderResource {
private:
	
	Ogre::GpuProgramPtr shader;
	GDBuilderContext* builderCxt;

	// Sets Up shader type and assigns ShaderVars for this shader
	void initShader(Ogre::MaterialPtr mat_p);

public:

	Shader(GDBuilderContext* builderCxt_p, Ogre::MaterialPtr mat, ShaderType shaderType_p) : 
		ShaderResource(ResourceHandler::GetInstance(), "", shaderType_p, "")
	{
		builderCxt = builderCxt_p;

		initShader(mat);

		
	}

	Ogre::GpuProgramPtr getShader() { return shader; }



};
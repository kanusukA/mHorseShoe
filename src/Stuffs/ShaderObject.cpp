#include "ShaderObject.h"


void Shader::initShader(Ogre::MaterialPtr mat_p)
{
	switch (shaderType)
	{
	case Vertex:
		shader = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgram();
		this->VertexShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgramName();
		break;
	case Fragment:
		shader = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgram();
		this->FragmentShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgramName();
		break;
	default:
		break;
	}

	builderCxt->setShaderVars(this);

}

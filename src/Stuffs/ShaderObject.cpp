#include "ShaderObject.h"


void Shader::initShader(Ogre::MaterialPtr mat_p)
{

	if(mat_p.get()->getTechnique(0)->getPass(0)->hasVertexProgram()){
		switch (shaderType)
		{
		case Vertex:
			shader = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters(); // humm
			this->VertexShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgramName();
			this->fileName = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getSourceFile();
			break;
		case Fragment:
			shader = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			this->FragmentShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgramName();
			this->fileName = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getSourceFile();
			break;
		default:
			break;
		}

		builderCxt->setShaderVars(this);
	}
	else {
		ToastComponent::GetInstance()->addMessage("No Programable shader!");
	}

}

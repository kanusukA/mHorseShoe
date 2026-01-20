#include "ShaderObject.h"


void Shader::_initShader(Ogre::MaterialPtr mat_p)
{

	if(mat_p.get()->getTechnique(0)->getPass(0)->hasVertexProgram()){
		switch (shaderType)
		{
		case Vertex:
			shader = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters(); // humm
			this->ShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgramName();
			this->fileName = mat_p.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getSourceFile();
			
			break;
		case Fragment:
			shader = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			this->ShaderName = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgramName();
			this->fileName = mat_p.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getSourceFile();
			break;
		default:
			break;
		}

		this->setName(ShaderName);
		
	}
	else {
		ToastComponent::GetInstance()->addMessage("No Programmable shader!");
	}

}

void Shader::_setShaderVars()
{
	std::filesystem::path shaderLoc = ResourceHandler::GetInstance()->fetchLocByFileName(this->fileName,ResourceLoaderEnums::Shaders);

	//Reading Shader File
	ResourceHandler::GetInstance()->readShaderFile(shaderLoc, this->ShaderParameters);


}

void Shader::loadShader()
{

}

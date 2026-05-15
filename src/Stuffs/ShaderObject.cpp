#include "ShaderObject.h"


void Shader::_initShader(Ogre::MaterialPtr mat_p)
{

	// SET TECHNIQUE TO 0 IF USING HLSL PROGRAMMING.
	// SET TECHNIQUE TO 1 IF USING GLSL PROGRAMMING. (I CREATE SECOND TECHNIQUE FOR GLSL PROGRAMS, WITH THE SAME SHADER VARS, BUT WITH GLSL SOURCE FILES INSTEAD OF HLSL) 
	int technique = mat_p.get()->getTechniques().size() > 1 ? 1 : 0;

	if(mat_p.get()->getTechnique(technique)->getPass(0)->hasVertexProgram()){
		switch (shaderType)
		{
		case Vertex:
			mat_p.get()->getTechnique(technique)->getPass(0)->getFragmentProgramParameters().get()->getSharedParameters();
			shaderParams = mat_p.get()->getTechnique(technique)->getPass(0)->getVertexProgramParameters(); // humm
			shaderProgram = mat_p.get()->getTechnique(technique)->getPass(0)->getVertexProgram();	
			this->ShaderName = mat_p.get()->getTechnique(technique)->getPass(0)->getVertexProgramName();
			this->fileName = mat_p.get()->getTechnique(technique)->getPass(0)->getVertexProgram().get()->getSourceFile();
			
			break;

		case Fragment:
			
			shaderParams = mat_p.get()->getTechnique(technique)->getPass(0)->getFragmentProgramParameters();
			shaderProgram = mat_p.get()->getTechnique(technique)->getPass(0)->getFragmentProgram();
			this->ShaderName = mat_p.get()->getTechnique(technique)->getPass(0)->getFragmentProgramName();
			this->fileName = mat_p.get()->getTechnique(technique)->getPass(0)->getFragmentProgram().get()->getSourceFile();

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
	std::cout << "LOAD SHADER STARTED!" << this->fileName << std::endl;

	std::filesystem::path shaderLoc = ResourceHandler::GetInstance()->fetchLocByFileName(this->fileName,ResourceLoaderEnums::Shaders);

	// TODO ADD AUTO SWITCH BETWEEN GLSL AND HLSL READER
	//Reading Shader File
	//ResourceHandler::GetInstance()->readShaderFile(shaderLoc, this->ShaderParameters);
	ResourceHandler::GetInstance()->readGLSLShaderFile(shaderLoc, this->ShaderParameters);
	// Implement a system by which shader value can be integrated at initialization!


}

void Shader::loadShaderVar(std::vector<ShaderVar> vars_p)
{
	// length check
	if (vars_p.size() != ShaderParameters->size())
	{
		ToastComponent::GetInstance()->addMessage("Shader : loadShaderVar() is inconsistant with shaderParameter. For : " + this->getName());
		ToastComponent::GetInstance()->addMessage("Shader : Merging Values");
	}

	bool found = false;

	for (int varsIndex = 0; varsIndex < vars_p.size(); varsIndex++)
	{
		found = false;
		for (int paramsIndex = 0; paramsIndex < ShaderParameters->size(); paramsIndex++)
		{
			if (vars_p.at(varsIndex).varType == ShaderParameters->at(paramsIndex).varType)
			{
				if (vars_p.at(varsIndex).varName == ShaderParameters->at(paramsIndex).varName)
				{
					found = true;
					switch (vars_p.at(varsIndex).varType)
					{
					case ShaderVarType::INTEGER:
						*ShaderParameters->at(paramsIndex).varInt = *vars_p.at(varsIndex).varInt;
						break;
					case ShaderVarType::FLOAT0:
						*ShaderParameters->at(paramsIndex).varFloat = *vars_p.at(varsIndex).varFloat;
						break;
					case ShaderVarType::FLOAT2:
						ShaderParameters->at(paramsIndex).varFloat2[0] = vars_p.at(varsIndex).varFloat2[0];
						ShaderParameters->at(paramsIndex).varFloat2[1] = vars_p.at(varsIndex).varFloat2[1];
						break;
					case ShaderVarType::FLOAT3:
						ShaderParameters->at(paramsIndex).varFloat3[0] = vars_p.at(varsIndex).varFloat3[0];
						ShaderParameters->at(paramsIndex).varFloat3[1] = vars_p.at(varsIndex).varFloat3[1];
						ShaderParameters->at(paramsIndex).varFloat3[2] = vars_p.at(varsIndex).varFloat3[2];
						break;
					case ShaderVarType::FLOAT4:
						ShaderParameters->at(paramsIndex).varFloat4[0] = vars_p.at(varsIndex).varFloat4[0];
						ShaderParameters->at(paramsIndex).varFloat4[1] = vars_p.at(varsIndex).varFloat4[1];
						ShaderParameters->at(paramsIndex).varFloat4[2] = vars_p.at(varsIndex).varFloat4[2];
						ShaderParameters->at(paramsIndex).varFloat4[3] = vars_p.at(varsIndex).varFloat4[3];
						break;
					default:
						break;
					}
				}
			}

		}
		if (!found)
		{
			ToastComponent::GetInstance()->addMessage("Shader: Var not found : " + vars_p.at(varsIndex).varName);
		}
	}

	GDBuilderCxt->monSetShaderVars(vars_p, shaderParams);
	// try this

}

void Shader::loadShader()
{

}

void Shader::_refreshShader()
{
	// ADD a rsus function to update only a shaderProgram with value, only limited to Shader class. ig use the GDBuilderCxt for that function!
}

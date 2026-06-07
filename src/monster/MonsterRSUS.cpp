#include <monster/Monster.h>


void RSUS::setShader(std::string matName,
	std::string fragShaderName,
	std::string vertShaderName,
	std::vector<ShaderVar>* fragShaderVar,
	std::vector<ShaderVar>* vertShaderVar,
	Ogre::GpuProgramParametersPtr fragProgram,
	Ogre::GpuProgramParametersPtr vertProgram
) {

	rsusObj->materialName = matName;
	rsusObj->fragShaderName = fragShaderName;
	rsusObj->vertShaderName = vertShaderName;
	rsusObj->fragProgramPtr = fragProgram;
	rsusObj->vertProgramPtr = vertProgram;
	rsusObj->fragVariables = fragShaderVar;
	rsusObj->vertVariables = vertShaderVar;

}

void RSUS::updateShaderVar(ShaderVar var, ShaderType shaderType)
{
	switch (var.varType)
	{
	case ShaderVarType::INTEGER:
		switch (shaderType)
		{
		case Vertex:
			updateVertParameterInt(var.varName, *var.varInt);
			break;
		case Fragment:
			updateFragParameterInt(var.varName, *var.varInt);
			break;
		default:
			break;
		}
		break;
	case ShaderVarType::FLOAT0:
		switch (shaderType)
		{
		case Vertex:
			updateVertParameterFloat(var.varName, var.varFloat);
			break;
		case Fragment:
			updateFragParameterFloat(var.varName, var.varFloat);
			break;
		default:
			break;
		}
		break;
	case ShaderVarType::FLOAT2:
		switch (shaderType)
		{
		case Vertex:
			updateVertParameterFloat2(var.varName, var.varFloat2);
			break;
		case Fragment:
			updateFragParameterFloat2(var.varName, var.varFloat2);
			break;
		default:
			break;
		}
		break;
	case ShaderVarType::FLOAT3:
		switch (shaderType)
		{
		case Vertex:
			updateVertParameterFloat3(var.varName, var.varFloat3);
			break;
		case Fragment:
			updateFragParameterFloat3(var.varName, var.varFloat3);
			break;
		default:
			break;
		}
		break;
	case ShaderVarType::FLOAT4:
		switch (shaderType)
		{
		case Vertex:
			updateVertParameterFloat4(var.varName, var.varFloat4);
			break;
		case Fragment:
			updateFragParameterFloat4(var.varName, var.varFloat4);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void RSUS::updateShaderVars(std::vector<ShaderVar> vars_p, Ogre::GpuProgramParametersPtr shaderPrgm)
{
	rsusObj->fragProgramPtr = shaderPrgm;

	for (int i = 0; i < vars_p.size(); i++)
	{
		updateShaderVar(vars_p.at(i), ShaderType::Fragment);
	}

}

void RSUS::readMaterial(Ogre::String matName, Ogre::String objectName)
{


	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(matName);

	Ogre::String fragProgramName = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getName();
	Ogre::String fragProgramFileName = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getSourceFile();
	Ogre::GpuProgramParametersPtr fragParam = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

	Ogre::String vertProgramName = mat.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getName();
	Ogre::String vertProgramFileName = mat.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getSourceFile();
	Ogre::GpuProgramParametersPtr vertParam = mat.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters();

	readTextures(mat);


	rsusObj->fragShaderName = fragProgramName;
	rsusObj->fragShaderFileName = fragProgramFileName;
	rsusObj->vertShaderName = vertProgramName;
	rsusObj->vertShaderFileName = vertProgramFileName;
	rsusObj->materialName = matName;
	rsusObj->fragProgramPtr = fragParam;
	rsusObj->vertProgramPtr = vertParam;

}

void RSUS::updateFragParameterInt(Ogre::String parameterName, int val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Int Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateFragParameterFloat(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val[0]);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val[0] << std::endl;
	}

}

void RSUS::updateFragParameterFloat2(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector2(val[0], val[1]));
	}
	catch (...) {
		std::cout << "Invalid Float2 Input For : " << parameterName << " Value : " << val << std::endl;
	}

}

void RSUS::updateFragParameterFloat3(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector3(val[0], val[1], val[2]));
	}
	catch (...) {
		std::cout << "Invalid Float3 Input For : " << parameterName << " Value : " << val << std::endl;
	}

}

void RSUS::updateFragParameterFloat4(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector4(val[0], val[1], val[2], val[3]));
	}
	catch (...) {
		std::cout << "Invalid Float4 Input For : " << parameterName << " Value : " << val << std::endl;
	}

}

void RSUS::updateFragParameterBool(Ogre::String parameterName, bool val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Bool Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterInt(Ogre::String parameterName, int val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterFloat(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, val[0]);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val[0] << std::endl;
	}
}

void RSUS::updateVertParameterFloat2(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector2(val[0], val[1]));
	}
	catch (...) {
		std::cout << "Invalid Float2 Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterFloat3(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector3(val[0], val[1], val[2]));
	}
	catch (...) {
		std::cout << "Invalid Float3 Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterFloat4(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector4(val[0], val[1], val[2], val[3]));
	}
	catch (...) {
		std::cout << "Invalid Float4 Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterBool(Ogre::String parameterName, bool val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Bool Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

RSUS* RSUS::GetInstance()
{
	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new RSUS();
	}
	return pinstance_;
}

void RSUS::setDiffuseTexture(Ogre::Texture* texture)
{
	if (texture->getName() == rsusObj->textures->Diffuse->getTextureName()) {
		std::cout << "Texture is applied!" << std::endl;
		return;
	}

	if (rsusObj->textures->Diffuse)
	{
		rsusObj->textures->Diffuse->setTexture(Ogre::TexturePtr(texture));
	}

}

void RSUS::setNormalTexture(Ogre::Texture* texture)
{

	if (texture->getName() == rsusObj->textures->Normal->getTextureName()) {
		std::cout << "Texture is applied!" << std::endl;
		return;
	}

	if (rsusObj->textures->Normal)
	{
		rsusObj->textures->Normal->setTexture(Ogre::TexturePtr(texture));
	}
}

void RSUS::setRoughnessTexture(Ogre::Texture* texture)
{

	if (texture->getName() == rsusObj->textures->Roughness->getTextureName()) {
		std::cout << "Texture is applied!" << std::endl;
		return;
	}

	if (rsusObj->textures->Roughness)
	{
		rsusObj->textures->Roughness->setTexture(Ogre::TexturePtr(texture));
	}
}

void RSUS::setParallaxTexture(Ogre::Texture* texture)
{
	if (texture->getName() == rsusObj->textures->Parallax->getTextureName()) {
		std::cout << "Texture is applied!" << std::endl;
		return;
	}

	if (rsusObj->textures->Parallax)
	{
		rsusObj->textures->Parallax->setTexture(Ogre::TexturePtr(texture));
	}
}

void RSUS::readTextures(Ogre::MaterialPtr mat)
{

	Ogre::Pass::TextureUnitStates textures = mat.get()->getTechnique(0)->getPass(0)->getTextureUnitStates();

	if (!textures.empty()) {
		for (int i = 0; i < textures.size(); i++)
		{

			if (textures.at(i)->getName() == NORMAL_TEX_NAME) {
				rsusObj->textures->Normal = textures.at(i);
			}
			if (textures.at(i)->getName() == DIFFUSE_TEX_NAME) {
				rsusObj->textures->Diffuse = textures.at(i);
			}
			if (textures.at(i)->getName() == ROUGH_TEX_NAME) {
				rsusObj->textures->Roughness = textures.at(i);
			}
			if (textures.at(i)->getName() == PARALLAX_TEX_NAME) {
				rsusObj->textures->Parallax = textures.at(i);
			}


		}
	}

}

ShaderVar RSUS::_putShaderValue(std::string valueStr)
{

	ShaderVar var = ShaderVar();

	std::string value = "";
	int floatPos = 0;

	std::cout << "Input : " << valueStr << std::endl;

	if (!valueStr.empty())
	{
		if (valueStr.at(0) == '0') {
			//std::cout << "value int : " << valueStr.substr(2, valueStr.size()) << std::endl;
			*var.varInt = std::stoi(valueStr.substr(2, valueStr.size()));
			var.varType = ShaderVarType::INTEGER;
		}
		else if (valueStr.at(0) == '1')
		{
			//std::cout << "value float : " << valueStr.substr(2, valueStr.size()) << std::endl;
			*var.varFloat = std::stof(valueStr.substr(2, valueStr.size()));
			var.varType = ShaderVarType::FLOAT0;
		}
		else if (valueStr.at(0) == '2')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					//std::cout << "value float2 : " << floatPos << " : " << value << std::endl;
					var.varFloat2[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			//std::cout << "value float2 : " << floatPos << " : " << value << std::endl;
			var.varFloat2[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT2;
		}
		else if (valueStr.at(0) == '3')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					//std::cout << "value float3 : " << floatPos << " : " << value << std::endl;
					var.varFloat3[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			//std::cout << "value float3 : " << floatPos << " : " << value << std::endl;
			var.varFloat3[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT3;
		}
		else if (valueStr.at(0) == '4')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					//std::cout << "value float4 : " << floatPos << " : " << value << std::endl;
					var.varFloat4[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			//std::cout << "value float4 : " << floatPos << " : " << value << std::endl;
			var.varFloat4[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT4;
		}
		else {
			std::cout << "Unknown Type in Save file" << std::endl;
		}
	}
	else {
		std::cout << "Invalid save file contents" << std::endl;
		// throw exception here
		throw ShaderSaveFileInconsistent();
	}

	return var;

}
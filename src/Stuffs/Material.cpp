#include "Material.h"


void Material::readTextures()
{
	textures = new std::vector<ShaderTexture>();
	for (int texIndex = 0; texIndex < material->getTechnique(0)->getPass(0)->getNumTextureUnitStates(); texIndex++)
	{
		ShaderTexture tex = ShaderTexture();

		tex.textureName = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->getName();
		tex.texturePosition = texIndex;
		tex.texture = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->_getTexturePtr();
		tex.addressingMode = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->getTextureAddressingMode().u;
		tex.scale = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->getTextureUScale();
	

		textures->push_back(tex);
	}
}

void Material::setTextureWrapMode(int texturePosition_p, Ogre::TextureUnitState::TextureAddressingMode mode_p)
{
	try
	{
		material->getTechnique(0)->getPass(0)->getTextureUnitState(texturePosition_p)->setTextureAddressingMode(mode_p);
		this->textures->at(texturePosition_p).addressingMode = mode_p;
	}
	catch (const std::exception&)
	{
		ToastComponent::GetInstance()->addMessage("Error Setting texture wrap mode");
	}
}

void Material::setTextureScale(int texturePosition_p, float scale)
{
	try
	{
		material->getTechnique(0)->getPass(0)->getTextureUnitState(texturePosition_p)->setTextureScale(scale, scale);
	}
	catch (const std::exception&)
	{
		ToastComponent::GetInstance()->addMessage("Error Setting texture scale");
	}
}

void Material::setTexture(int texturePosition_p, Ogre::TexturePtr tex_p)
{
	
	try
	{
		material->getTechnique(0)->getPass(0)->getTextureUnitState(texturePosition_p)->setTexture(tex_p);
		this->textures->at(texturePosition_p).textureName = tex_p.get()->getName();
		this->textures->at(texturePosition_p).texture = tex_p;
	}
	catch (const std::exception&)
	{
		ToastComponent::GetInstance()->addMessage("Error Setting texture");
	}
}

void Material::setCullingMode(Ogre::CullingMode culling_p) {
	culling = culling_p;
	material->getTechnique(0)->getPass(0)->setCullingMode(culling);
}

void Material::setWireFrameMode(bool mode_p)
{
	wireframeMode = mode_p;
	material->getTechnique(0)->getPass(0)->setPolygonMode( wireframeMode ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID);
}

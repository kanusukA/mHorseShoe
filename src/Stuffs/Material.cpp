#include "Material.h"


void Material::readTextures()
{
	textures = new std::vector<ShaderTexture>(material->getTechnique(0)->getPass(0)->getNumTextureUnitStates());
	for (int texIndex = 0; texIndex < material->getTechnique(0)->getPass(0)->getNumTextureUnitStates(); texIndex++)
	{
		ShaderTexture tex = ShaderTexture();

		tex.textureName = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->getName();
		tex.texturePosition = texIndex;
		tex.texture = material->getTechnique(0)->getPass(0)->getTextureUnitState(texIndex)->_getTexturePtr();

		textures->push_back(tex);
	}
}

void Material::setTexture(int texturePosition_p, Ogre::TexturePtr tex_p)
{
	if (material->getTechnique(0)->getPass(0)->getTextureUnitState(texturePosition_p))
	{
		material->getTechnique(0)->getPass(0)->getTextureUnitState(texturePosition_p)->setTexture(tex_p);
	}
	else {
		ToastComponent::GetInstance()->addMessage("Error Setting texture");
	}
}

void Material::setCullingMode(Ogre::CullingMode culling_p) {
	culling = culling_p;
	material->getTechnique(0)->getPass(0)->setCullingMode(culling);
}

void Material::setWireFrameMode(bool mode_p)
{
	wireFrameMode = mode_p;
	material->getTechnique(0)->getPass(0)->setPolygonMode( wireFrameMode ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID);
}

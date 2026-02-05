#include "Material.h"


void Material::readTextures()
{
	for (int i = 0; i < material->getTechnique(0)->getPass(0)->getNumTextureUnitStates(); i++)
	{
		ToastComponent::GetInstance()->addMessage("MaterialTexture : " + material->getTechnique(0)->getPass(0)->getTextureUnitState(i)->getTextureName());
		ShaderTexture texture = ShaderTexture();

		texture.textureName = material->getTechnique(0)->getPass(0)->getTextureUnitState(i)->getName();
		texture.texturePosition = i;
		texture.texture = material->getTechnique(0)->getPass(0)->getTextureUnitState(i)->getTextureName();

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

void MaterialDeletor(Material* material_p)
{

}

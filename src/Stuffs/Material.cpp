#include "Material.h"


void Material::setCullingMode(Ogre::CullingMode culling_p) {
	culling = culling_p;
	material->getTechnique(0)->getPass(0)->setCullingMode(culling);
}

void Material::setWireFrameMode(bool mode_p)
{
	wireFrameMode = mode_p;
	material->getTechnique(0)->getPass(0)->setPolygonMode( wireFrameMode ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID);
}

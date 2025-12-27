#include <GDHandler/GDHandler.h>

Ogre::SceneNode* GDHandler::createScene(std::string name_p) {
	return monster->addToScnNode(name_p, monster->oScnManager->getRootSceneNode());
}

Ogre::Entity* GDHandler::createObject(std::string name_p)
{
	return monster->createEntity(name_p);
}

Ogre::MaterialPtr GDHandler::createMaterial(std::string materialName_p)
{
	return monster->getMaterial(materialName_p);
}

ResID GDHandler::addMaterial(std::string matname_p)
{
	return caseHandler->CreateMaterial(matname_p)->getId();
}

ResID GDHandler::addShader(Ogre::MaterialPtr mat_p, ShaderType type)
{
	return caseHandler->CreateShader(mat_p,type)->getId();
}

void GDHandler::setShaderVars(Shader* shader)
{
	// TODO IMPLEMENT NEW RSUS METHOD
	
}

Ogre::Mesh* GDHandler::createMesh(std::string meshName)
{
	return monster->getMesh(meshName);
}

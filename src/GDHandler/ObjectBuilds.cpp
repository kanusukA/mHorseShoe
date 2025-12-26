#include <GDHandler/GDHandler.h>

Ogre::SceneNode* GDHandler::createScene(std::string name_p) {
	return monster->addToScnNode(name_p, monster->oScnManager->getRootSceneNode());
}

Ogre::Entity* GDHandler::createObject(std::string name_p)
{
	return monster->createEntity(name_p);
}

Ogre::Mesh* GDHandler::createRenderMesh(std::string meshName)
{
	return 
}

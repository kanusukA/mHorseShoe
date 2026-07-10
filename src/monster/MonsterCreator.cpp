#include <monster/Monster.h>

// ALL MONSTER FUNCTIONS RELATED TO CREATING OR FETCHING OBJECTS

Ogre::Entity* Monster::createMeshEntity(Ogre::String mshname, Ogre::String groupName)
{
	if (this->resourceGroupExists(groupName) && this->resourceExists(mshname,groupName))
	{
		Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(mshname, groupName);
		return oScnManager->createEntity(msh);
	}
	return nullptr;
	
}

Ogre::Entity* Monster::createMeshEntity(Ogre::String entityname, Ogre::String mshname, Ogre::String groupName)
{
	if (this->resourceGroupExists(groupName) && this->resourceExists(mshname, groupName) && !this->resourceEntityExists(entityname))
	{
		Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(mshname, groupName);
		return oScnManager->createEntity(entityname,msh);
	}
	return nullptr;
}

Ogre::Entity* Monster::createEntity(Ogre::String entityName_p, Ogre::MeshPtr mesh_p)
{
	return oScnManager->createEntity(entityName_p, mesh_p);
}

Ogre::MaterialPtr Monster::createEmptyMaterial(std::string name_p, Ogre::String groupName)
{
	if (this->resourceGroupExists(groupName) && !this->resourceExists(name_p, groupName))
	{
		return Ogre::MaterialManager::getSingletonPtr()->create(name_p, groupName);
	}
	return nullptr;
}

Ogre::SceneNode* Monster::createNewScnNodeAttach(std::string scnNodeName, Ogre::SceneNode* node)
{
	Ogre::SceneNode* new_scnNode = node->createChildSceneNode(scnNodeName);
	return new_scnNode;
}


Ogre::MeshPtr Monster::getMesh(Ogre::String meshName, Ogre::String groupName)
{
	// Ogre starts phrasing everything inside the foldler with is not only wasteful but can be used to inject/run unintended files!
	//Ogre::ResourceGroupManager
	if (this->resourceGroupExists(groupName) && this->resourceExists(meshName,groupName)) //this->resourceExists(meshName, groupName))
	{
		return Ogre::MeshManager::getSingleton().load(meshName, groupName);
	}
	return nullptr;
	
}

Ogre::MaterialPtr Monster::getMaterial(Ogre::String matName_p, Ogre::String groupName)
{
	if (this->resourceGroupExists(groupName) && this->resourceExists(matName_p, groupName))
	{
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(matName_p, groupName);
		return mat;
	}
	return nullptr;
	
}


Ogre::SceneNode* Monster::addToScnNode(Ogre::String meshName, Ogre::SceneNode* toScnNode)
{
	
	Ogre::Entity* entity = this->createMeshEntity(meshName);
	if (entity)
	{
		Ogre::SceneNode* scnNode = toScnNode->createChildSceneNode();
		scnNode->attachObject(entity);
		return scnNode;
	}
	return nullptr;
	
}

Ogre::TexturePtr Monster::getImageTexture(std::string textureName, Ogre::String groupName)
{
	if (Ogre::TextureManager::getSingleton().resourceExists(textureName,groupName))
	{
		return Ogre::TextureManager::getSingleton().getByName(textureName);
	}

	return nullptr;

	
}


// DELETION FUNCTION

void Monster::rmSceneNode(std::string scnNode)
{
	if (oScnManager->hasSceneNode(scnNode))
	{
		oScnManager->destroySceneNode(scnNode);
	}

}

void Monster::rmEntity(std::string entName)
{
	if (oScnManager->hasEntity(entName))
	{

		oScnManager->destroyEntity(entName);
	}
}

void Monster::rmMaterial(std::string matName, std::string groupName)
{
	if (this->resourceGroupExists(groupName) && this->resourceExists(matName,groupName))
	{
		Ogre::MaterialManager::getSingleton().remove(matName, groupName);
	}
}
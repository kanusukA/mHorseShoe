#include "GDHandler.h"



void GDHandler::loadSavedCase() {
	std::string loadfile = this->loadDefaultCase();
	if (loadfile.empty())
	{
		ToastComponent::GetInstance()->addMessage("No default Case found.");
		return;
	}
	if (resourceHandler->fileExists(this->resourceHandler->getDataDir().string() + "/" + loadfile))
	{
		this->loadCase(this->resourceHandler->getDataDir().string() + "/" + loadfile);
		if (this->caseVec && this->caseVec->size() > 0)
		{
			ToastComponent::GetInstance()->addMessage("LOADED DEFAULT CASE");
		}
		else {
			ToastComponent::GetInstance()->addMessage("Error Loading Default case file.");
		}
	}
	else {
		ToastComponent::GetInstance()->addMessage("Unable to open : " + this->resourceHandler->getDataDir().string() + "/" + loadfile);
	}
	
}

//void GDHandler::setResources()
//{
//	// Textures
//	if (resourceHandler->getTexturesLoaded())
//	{
//		for (int i = 0; i < resourceHandler->getTexturesLoaded()->size(); i++)
//		{
//			caseHandler->CreateImage(resourceHandler->getTexturesLoaded()->at(i));
//		}
//
//
//	}
//
//
//
//
//	// Material
//	if (resourceHandler->getMaterialsLoaded())
//	{
//		// check if Ogre resource grp exists
//		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceGroupExists(OGRE_MATERIAL_GROUP))
//		{
//			Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(OGRE_MATERIAL_GROUP);
//		}
//		
//
//		for (int i = 0; i < resourceHandler->getMaterialsLoaded()->size(); i++)
//		{
//			
//			// Load Material Path with Ogre
//			if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(resourceHandler->getMaterialsLoaded()->at(i).parent_path().string(),OGRE_MATERIAL_GROUP))
//			{
//				Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(resourceHandler->getMaterialsLoaded()->at(i).parent_path().string(), "FileSystem", OGRE_MATERIAL_GROUP);
//			}
//			
//
//		}
//
//		Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
//
//		for (int i = 0; i < resourceHandler->getMaterialsLoaded()->size(); i++)
//		{
//			std::string matName = resourceHandler->readMaterialName(resourceHandler->getMaterialsLoaded()->at(i));
//
//			ToastComponent::GetInstance()->addMessage("Loading Material : " + matName);
//
//			Material* mat = caseHandler->CreateMaterial(matName);
//
//			Shader* vertShader = caseHandler->CreateShader(mat->getMaterialPtr(), ShaderType::Vertex);
//			Shader* fragShader = caseHandler->CreateShader(mat->getMaterialPtr(), ShaderType::Fragment);
//
//			mat->setVertexShader(vertShader);
//			mat->setFragmentShader(fragShader);
//
//
//		}
//	}
//
//	if (resourceHandler->getRenderMeshLoaded())
//	{
//		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceGroupExists(OGRE_MESH_GROUP))
//		{
//			Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(OGRE_MESH_GROUP);
//		}
//
//		for (int i = 0; i < resourceHandler->getRenderMeshLoaded()->size(); i++)
//		{
//			if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(resourceHandler->getRenderMeshLoaded()->at(i).parent_path().string(), OGRE_MESH_GROUP))
//			{
//				Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(resourceHandler->getRenderMeshLoaded()->at(i).parent_path().string(), "FileSystem", OGRE_MESH_GROUP);
//			}
//
//			RenderMesh* mesh = caseHandler->CreateRenderMesh(resourceHandler->getRenderMeshLoaded()->at(i).filename().string());
//
//		}
//
//	}
//}





void GDHandler::update(float deltatime)
{
	feel->updateInput(deltatime);

	if (playerSubject && this->getWindowState() == GDSun::IN_GAME)
	{
		playerSubject->updatePosition(deltatime);
		playerSubject->updateRotation(deltatime);
	}

	gui->updateGui();

	//
	// kint->updatePhysics(deltatime);

	// updates Monster and Kint
	//stuffHandler->update(deltatime);

	monster->updateMonster();




}


//void GDHandler::_updatePlayerPrameters(float deltaTime)
//{
//	// PLAYER MOVEMENT
//	// Updates the player Scene node set in the monster player Node
//	// Updates the playerSubject which in turn updates all the assigned Observers
//	if (playerSubject != nullptr && mouseHidden) {
//		updateRotation(deltaTime, playerSubject);
//		updatePosition(deltaTime, playerSubject);
//	}
//
//}



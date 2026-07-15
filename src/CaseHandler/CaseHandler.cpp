#include "CaseHandler.h"



std::unique_ptr<Case> CaseHandler::selectedCase;

/*bool CaseHandler::resourceExists(std::string resourceName)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(resourceName);
}
*/
Case* CaseHandler::CreateCase(std::string caseName_p)
{
	Case* newCase = new Case(this, caseName_p);
	return newCase;

}

Case* CaseHandler::CreateCase(std::string caseName_p, std::string filename_p)
{
	Case* newCase = new Case(this, caseName_p, filename_p);
	return newCase;
}


/*Scene* CaseHandler::CreateScene(std::string scnName, SceneType scnType, Ogre::SceneNode* parentNode_p)
{
	if (this->sceneExists(scnName))
	{
		ToastComponent::GetInstance()->addMessage("Scene Already exists!!");
		return nullptr;
	}
	Ogre::SceneNode* sceneNode = monCreateSceneNode(scnName,parentNode_p);
	Scene* new_scn = new Scene(this, scnType, scnName,sceneNode);
	return new_scn;
	
}

Scene* CaseHandler::CreateScene(std::string scnName, SceneType scnType)
{
	if (scnName.empty())
	{
		ToastComponent::GetInstance()->addMessage("Scene Name is Empty!!");
		return nullptr;
	}
	if (this->sceneExists(scnName))
	{
		ToastComponent::GetInstance()->addMessage("Scene Already exists!!");
		return nullptr;
	}
	Ogre::SceneNode* sceneNode = monCreateSceneNode(scnName, oScnManager->getRootSceneNode());
	Scene* new_scn = new Scene(this, scnType, scnName, sceneNode);
	return new_scn;
}

Object* CaseHandler::CreateObject(std::string objectName_p, std::filesystem::path meshPath_p ,  PhysXType type)
{
	
	// Get RenderMesh
	Ogre::MeshPtr mesh = fetchMeshByName(meshPath_p);
	if (!mesh)
	{
		ToastComponent::GetInstance()->addMessage("Unable to create Object invaild Mesh");
		return nullptr;
	}

	if (!this->objectExists(objectName_p))
	{
		Ogre::Entity* ent = this->monCreateEntity(objectName_p, mesh);
		Object* new_obj = new Object(this, ent, objectName_p, type,meshPath_p);
		return new_obj;

	}
	ToastComponent::GetInstance()->addMessage("Object already exists");
	return nullptr;
	
}

Ogre::MeshPtr CaseHandler::fetchMeshByName(std::filesystem::path meshPath_p)
{

	//this->monAddLocation(meshPath_p.parent_path(), OGRE_MESH_GROUP); // must get initialized once a new path is added!
	Ogre::MeshPtr mesh = this->monGetMesh(meshPath_p.filename().string());

	if (!mesh)
	{
		ToastComponent::GetInstance()->addMessage("Mesh file : " + meshPath_p.filename().string() + " does not exist! ");
		return nullptr;
	}

	return mesh;
}

Ogre::MeshPtr CaseHandler::fetchMeshById(ResID meshID_p)
{
	for (int i = 0; i < meshVec->size(); i++)
	{
		if (meshVec->at(i)->getId() == meshID_p)
		{
			return meshVec->at(i)->getMesh();
		}
	}

	return nullptr;
}
*/
/*Ogre::TexturePtr CaseHandler::fetchImageByName(std::filesystem::path imagePath_p)
{
	this->monAddLocation(imagePath_p.parent_path(), OGRE_TEXTURE_GROUP);
	this->monster->initalizeResourceGroup(OGRE_TEXTURE_GROUP);

	// Duplicate Resource is checked!
	return this->monGetTexture(imagePath_p.filename().string());


}


Material* CaseHandler::CreateMaterial(std::filesystem::path materialPath_p, std::string materialName)
{

	Ogre::MaterialPtr ogreMaterial = monCreateMaterial(resourceHandler->readMaterialName(materialPath_p));

	if (ogreMaterial.isNull())
	{
		return nullptr;
	}

	Ogre::MaterialPtr newMat = monCreateNewMaterial(materialName);
	if (newMat)
	{
		ogreMaterial->copyDetailsTo(newMat);
		Material* mat = new Material(this, newMat,materialPath_p.string());

		return mat;
	}
	else {
		return nullptr;
	}
	

}


Image* CaseHandler::CreateImage(std::filesystem::path filePath_p)
{
	if (std::filesystem::exists(filePath_p))
	{
		Image* new_img = new Image(filePath_p);
		return new_img;
	}
	return nullptr;
	
}

void CaseHandler::unload()
{
	selectedCase.reset();
}


void CaseHandler::loadCase(std::filesystem::path yamlFilePath)
{
	RLCase rlCase = resourceHandler->fetchCaseData(yamlFilePath);
	
	Case* wCase = CreateCase(rlCase.name,yamlFilePath.filename().string());

	if (!wCase)
	{
		ToastComponent::GetInstance()->addMessage("Failed To Load Case : " + rlCase.name + "Failed to create case resource!");
		return;
	}

	for (int scenesIndex = 0; scenesIndex < rlCase.Scenes.size(); scenesIndex++)
	{
		std::weak_ptr<Scene> wScene = wCase->attachNewSceneToRoot(rlCase.Scenes.at(scenesIndex).name,SceneType(rlCase.Scenes.at(scenesIndex).scnType));
		if (wScene.expired())
		{
			ToastComponent::GetInstance()->addMessage("Failed To Load Case : " + wCase->getName() + "Failed to create scene : " + rlCase.Scenes.at(scenesIndex).name);
			return;
		}

		for (int objIndex = 0; objIndex < rlCase.Scenes.at(scenesIndex).objects.size(); objIndex++)
		{
			RLObject obj = rlCase.Scenes.at(scenesIndex).objects.at(objIndex);

			std::weak_ptr<Object> wObj =  wScene.lock()->attachNewObject(obj.name, obj.mesh.filepath, PhysXType(obj.ObjectPhysxType));
			
			if (wObj.expired())
			{
				ToastComponent::GetInstance()->addMessage("Failed To Load Case : " + wCase->getName() + "Failed to create object : " + obj.name + " in Scene : " + rlCase.Scenes.at(scenesIndex).name);
				return;
			}

			if (wObj.lock()->setMaterial(obj.material.materialFilePath, obj.material.name))
			{
				ToastComponent::GetInstance()->addMessage("culling - " + std::to_string(obj.material.culling));
				wObj.lock()->getwMaterial().lock()->setCullingMode(obj.material.culling);
				wObj.lock()->getwMaterial().lock()->setWireFrameMode(obj.material.wireframe);

				if (!obj.material.vertShader.shaderVars.empty())
				{
					wObj.lock()->getwMaterial().lock()->getVertexShader()->loadShaderVar(obj.material.vertShader.shaderVars);
				}

				if (!obj.material.fragShader.shaderVars.empty())
				{
					wObj.lock()->getwMaterial().lock()->getFragmentShader()->loadShaderVar(obj.material.fragShader.shaderVars);
				}
			}
			

		}

		// Scene Scale/Position/Rotation is set after objects have been added. As Objects under the scene will not reflect value set to the scene prior.

		

		wScene.lock()->setScale(Ogre::Vector3(rlCase.Scenes.at(scenesIndex).scale[0], rlCase.Scenes.at(scenesIndex).scale[1],
			rlCase.Scenes.at(scenesIndex).scale[2]));

		wScene.lock()->setPosition(Ogre::Vector3(rlCase.Scenes.at(scenesIndex).position[0], rlCase.Scenes.at(scenesIndex).position[1],
			rlCase.Scenes.at(scenesIndex).position[2]));

		wScene.lock()->setOrientation(Ogre::Quaternion(rlCase.Scenes.at(scenesIndex).rotation[0], rlCase.Scenes.at(scenesIndex).rotation[1],
			rlCase.Scenes.at(scenesIndex).rotation[2], rlCase.Scenes.at(scenesIndex).rotation[3]));

		
		

	}

	selectedCase.reset(wCase);
	wCase = nullptr;

	// notify GDNotifier that new csae has been create, which will trigger GUI update to reflect new case in the UI.
	this->notifyLoadCase();

	ToastComponent::GetInstance()->addMessage("Loaded");
}

std::string CaseHandler::loadDefaultCase()
{
	if (resourceHandler->openLoadFile(resourceHandler->getSourceDir().string() + CASE_DEFAULT_CASE_PATH))
	{
		return  resourceHandler->fetchNodeByMapKey(CASE_DEFAULT_CASE_NAME);
	}
	return "";



}*/







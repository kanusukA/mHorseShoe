#include "CaseHandler.h"

void CaseHandler::checkIntegrity()
{
	// Check Scenes in 
	//Ogre::Node::ChildNodeMap nodes = oScnManager->getRootSceneNode()->getChildren();

	////std::vector<SceneResource*>* scnNodes = ResourceHandler::GetInstance()->getAllScenes();
	//bool match = true;
	//if (nodes.size() == scnNodes->size())
	//{
	//	for (int i = 0; i < nodes.size(); i++)
	//	{
	//		match = false;
	//		for (int j = 0; j < scnNodes->size(); j++)
	//		{
	//			if (nodes.at(i)->getName() != scnNodes->at(j)->getName())
	//			{
	//				match = true;
	//			}
	//		}
	//		if (!match)
	//		{
	//			throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
	//		}
	//	}
	//}
	//else {
	//	throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
	//}
	//

}


std::weak_ptr<Case> CaseHandler::CreateCase(std::string caseName_p)
{
	std::shared_ptr<Case> sCase = std::make_shared<Case>(this,caseName_p);
	caseVec->push_back(std::move(sCase));

	return caseVec->at(caseVec->size() - 1);

}


Scene* CaseHandler::CreateScene(std::string scnName, SceneType scnType, Ogre::SceneNode* parentNode_p)
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
	// Check if it already exists
	/*for (int i = 0; i < meshVec->size(); i++)
	{
		if (meshVec->at(i)->getMeshName() == meshPath_p.filename().string())
		{
			return meshVec->at(i)->getMesh();
		}
	}*/
	
	// Create if it does not exist
	this->monSetLocation(meshPath_p.parent_path(), OGRE_MESH_GROUP);
	Ogre::MeshPtr mesh = this->monGetMesh(meshPath_p.filename().string());
	
	//std::unique_ptr<RenderMesh> uMesh = std::make_unique<RenderMesh>(this,mesh);
	//meshVec->push_back(std::move(uMesh));

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

Ogre::TexturePtr CaseHandler::fetchImageByName(std::filesystem::path imagePath_p)
{
	this->monSetLocation(imagePath_p.parent_path(), OGRE_TEXTURE_GROUP);
	this->monster->initalizeResourceGroup(OGRE_TEXTURE_GROUP);

	return this->monGetTexture(imagePath_p.filename().string());

}


Material* CaseHandler::CreateMaterial(std::filesystem::path materialPath_p, std::string materialName)
{
	// Check if Material is already set in stored vector
	//for (int i = 0; i < materialVec->size(); i++)
	//{
	//	if (materialVec->at(i)->getName() == materialPath_p.filename().string())
	//	{
	//		// Copy Material from stored place
	//		Ogre::MaterialPtr newMat = this->monCreateNewMaterial(materialName);
	//		materialVec->at(i)->getMaterialPtr().get()->copyDetailsTo(newMat);

	//		Material* mat = new Material(this, newMat);
	//		return mat;
	//	}
	//}

	// if not stored
	//this->monSetLocation(materialPath_p.parent_path(), OGRE_MATERIAL_GROUP);
	Ogre::MaterialPtr ogreMaterial = monCreateMaterial(resourceHandler->readMaterialName(materialPath_p));
	/*std::unique_ptr<Material> uMat = std::make_unique<Material>(this, ogreMaterial);
	materialVec->push_back(std::move(uMat));*/

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


//Shader* CaseHandler::CreateShader(Ogre::MaterialPtr mat_p, ShaderType type)
//{
//	if (mat_p)
//	{
//		Shader* new_shader = new Shader(this, mat_p, type);
//		return new_shader;
//	}
//	else {
//		ToastComponent::GetInstance()->addMessage("Invalid MaterialPtr provided");
//	}
//	
//}




Image* CaseHandler::CreateImage(std::filesystem::path filePath_p)
{
	Image* new_img = new Image(filePath_p);
	return new_img;
}



void CaseHandler::loadSavedResource()
{
	// Cases
	//if (ResourceHandler::GetInstance()->RLCases->size() > 0)
	//{
	//	// the first case
	//	Case* savedCase = new Case(this, ResourceHandler::GetInstance()->RLCases->at(0).name);

	//
	//	for (int i = 0; i < ResourceHandler::GetInstance()->RLCases->at(0).Scenes.size(); i++)
	//	{
	//		for (int j = 0; j < ResourceHandler::GetInstance()->RLScenes->size(); j++)
	//		{
	//			if (ResourceHandler::GetInstance()->RLScenes->at(j).id == ResourceHandler::GetInstance()->RLCases->at(0).Scenes.at(i))
	//			{
	//				RLScene* scn = &ResourceHandler::GetInstance()->RLScenes->at(j);
	//				Scene* new_scn = new Scene(this, SceneType(scn->scnType), scn->name, convertFloatPtrToVec3(scn->position), convertFloatPtrToVec4(scn->rotation),
	//					convertFloatPtrToVec3(scn->scale));
	//				savedCase->addSceneToCase(new_scn);

	//				// load Objects
	//				if (scn->objects.size() > 0)
	//				{
	//					for (int objindex = 0; objindex < scn->objects.size(); objindex++)
	//					{
	//						for (int RlobjIndex = 0; RlobjIndex < ResourceHandler::GetInstance()->RLObjects->size(); RlobjIndex++)
	//						{
	//							if (scn->objects.at(objindex) == ResourceHandler::GetInstance()->RLObjects->at(RlobjIndex).id)
	//							{
	//								RLObject* rl_obj = &ResourceHandler::GetInstance()->RLObjects->at(RlobjIndex);

	//								RLFetchedResource* rlResources = ResourceHandler::GetInstance()->fetchResourcesFromMesh(rl_obj->renderMeshID);
	//								if (rlResources)
	//								{
	//									


	//									//Object* newObj = new Object(this->builderCxt, )
	//								}
	//								

	//							}
	//						}
	//					}

	//				}

	//			}
	//		}
	//		
	//	}
	//	

	//}

}

void CaseHandler::loadCase(std::filesystem::path yamlFilePath)
{
	RLCase rlCase = resourceHandler->fetchCaseData(yamlFilePath);
	
	std::weak_ptr<Case> wCase = CreateCase(rlCase.name);

	for (int scenesIndex = 0; scenesIndex < rlCase.Scenes.size(); scenesIndex++)
	{
		std::weak_ptr<Scene> wScene = wCase.lock()->attachNewSceneToRoot(rlCase.Scenes.at(scenesIndex).name,SceneType(rlCase.Scenes.at(scenesIndex).scnType));

		for (int objIndex = 0; objIndex < rlCase.Scenes.at(scenesIndex).objects.size(); objIndex++)
		{
			RLObject obj = rlCase.Scenes.at(scenesIndex).objects.at(objIndex);

			std::weak_ptr<Object> wObj =  wScene.lock()->attachNewObject(obj.name, obj.mesh.filepath, PhysXType(obj.ObjectPhysxType));

			wObj.lock()->setMaterial(obj.material.materialFilePath, obj.material.name);
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

		// Scene Scale/Position/Rotation is set after objects have been added. As Objects under the scene will not reflect value set to the scene prior.

		

		wScene.lock()->setScale(Ogre::Vector3(rlCase.Scenes.at(scenesIndex).scale[0], rlCase.Scenes.at(scenesIndex).scale[1],
			rlCase.Scenes.at(scenesIndex).scale[2]));

		wScene.lock()->setPosition(Ogre::Vector3(rlCase.Scenes.at(scenesIndex).position[0], rlCase.Scenes.at(scenesIndex).position[1],
			rlCase.Scenes.at(scenesIndex).position[2]));

		wScene.lock()->setOrientation(Ogre::Quaternion(rlCase.Scenes.at(scenesIndex).rotation[0], rlCase.Scenes.at(scenesIndex).rotation[1],
			rlCase.Scenes.at(scenesIndex).rotation[2], rlCase.Scenes.at(scenesIndex).rotation[3]));

		

	}


	ToastComponent::GetInstance()->addMessage("Loaded");
}




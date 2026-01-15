#include "CaseHandler.h"

void CaseHandler::checkIntegrity()
{
	// Check Scenes in 
	Ogre::Node::ChildNodeMap nodes = oScnManager->getRootSceneNode()->getChildren();

	std::vector<SceneResource*>* scnNodes = ResourceHandler::GetInstance()->getAllScenes();
	bool match = true;
	if (nodes.size() == scnNodes->size())
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			match = false;
			for (int j = 0; j < scnNodes->size(); j++)
			{
				if (nodes.at(i)->getName() != scnNodes->at(j)->getName())
				{
					match = true;
				}
			}
			if (!match)
			{
				throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
			}
		}
	}
	else {
		throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
	}
	

}


Case* CaseHandler::CreateCase(std::string caseName_p)
{
	Case* new_case = new Case(this,caseName_p);
	this->cases->push_back(new_case);
	this->currentCase = new_case;
	return new_case;
}


Scene* CaseHandler::CreateScene(SceneType scnType, std::string scnName)
{
	if (this->sceneExists(scnName))
	{
		ToastComponent::GetInstance()->addMessage("Scene Already exists!!");
		return nullptr;
	}
	else {
		Scene* new_scn = new Scene(this, scnType, scnName);
		//this->currentCase->addSceneToCase(new_scn);
		return new_scn;
	}
	
	
}

// Creates and adds sceneNode to current Case
Scene* CaseHandler::CreateSceneAttachToCase(SceneType scnType, std::string scnName)
{
	Scene* new_scn = this->CreateScene(scnType, scnName);
	if (new_scn)
	{
		currentCase->addSceneToCase(new_scn);
		return new_scn;
	}
	return nullptr;
}

Object* CaseHandler::CreateObject(std::string objectName_p, RenderMesh* renderMesh_p ,  PhysXType type)
{
	if (!this->objectExists(objectName_p))
	{
		Object* new_obj = new Object(this, renderMesh_p, objectName_p, type);
		
		return new_obj;
	}
	ToastComponent::GetInstance()->addMessage("Object already exists");
	return nullptr;
	
	
}

// old method
//RenderMesh* CaseHandler::CreateRenderMesh(std::string meshName_p)
//{
//	RenderMesh* new_msh = new RenderMesh(this->builderCxt, meshName_p);
//	return new_msh;
//}

RenderMesh* CaseHandler::CreateRenderMesh(std::filesystem::path path_p)
{

	// Check if Render Mesh already Exists
	ResID id = ResourceHandler::GetInstance()->doesRenderMeshExists(path_p.filename().string());

	if (id > 0) // Render Mesh EXIST
	{
		return static_cast<RenderMesh*>(ResourceHandler::GetInstance()->fetchRenderMeshResourceByID(id));
	}
	else { // Render Mesh DOES NOT EXIST
		
		// initialize location from ogre/monster
		getMonsterRef()->addOgreResourceLocation(path_p.parent_path().string(), OGRE_MESH_GROUP);

		// Create RenderMesh
		RenderMesh* renderMesh = new RenderMesh(this, path_p.filename().string());
		return renderMesh;

	}

}

ColliderMesh* CaseHandler::CreateColliderMesh(std::string MeshName_p)
{
	ColliderMesh* new_msh = new ColliderMesh(this, MeshName_p);
	return new_msh;
}

Shader* CaseHandler::CreateShader(Ogre::MaterialPtr mat_p, ShaderType type)
{
	if (mat_p)
	{
		Shader* new_shader = new Shader(this, mat_p, type);
		return new_shader;
	}
	else {
		ToastComponent::GetInstance()->addMessage("Invalid MaterialPtr provided");
	}
	
}

ResID CaseHandler::CreateMaterialResource(std::filesystem::path path_p)
{
	// Check if Exists
	ResID id = ResourceHandler::GetInstance()->doesMaterialExists(path_p.filename().string());

	if (id == 0)
	{
		// Creating a Copy of Material and Making it a unique_ptr to Object
		monster->addOgreResourceLocation(path_p.parent_path().string(), OGRE_MATERIAL_GROUP);

		return createMaterialResource(path_p.filename().string());

	}

	return 0;
	
}

Material* CaseHandler::getMaterial(std::filesystem::path materialPath_p)
{
	ResID materialResID = CreateMaterialResource(materialPath_p);

	Ogre::MaterialPtr material = createMaterial(materialPath_p.filename().string());

	return fetchNewMaterial(materialResID, material);

}

Image* CaseHandler::CreateImage(std::filesystem::path filePath_p)
{
	Image* new_img = new Image(filePath_p);
	return new_img;
}



void CaseHandler::loadSavedResource()
{
	// Cases
	if (ResourceHandler::GetInstance()->RLCases->size() > 0)
	{
		// the first case
		Case* savedCase = new Case(this, ResourceHandler::GetInstance()->RLCases->at(0).name);

	
		for (int i = 0; i < ResourceHandler::GetInstance()->RLCases->at(0).Scenes.size(); i++)
		{
			for (int j = 0; j < ResourceHandler::GetInstance()->RLScenes->size(); j++)
			{
				if (ResourceHandler::GetInstance()->RLScenes->at(j).id == ResourceHandler::GetInstance()->RLCases->at(0).Scenes.at(i))
				{
					RLScene* scn = &ResourceHandler::GetInstance()->RLScenes->at(j);
					Scene* new_scn = new Scene(this, SceneType(scn->scnType), scn->name, convertFloatPtrToVec3(scn->position), convertFloatPtrToVec4(scn->rotation),
						convertFloatPtrToVec3(scn->scale));
					savedCase->addSceneToCase(new_scn);

					// load Objects
					if (scn->objects.size() > 0)
					{
						for (int objindex = 0; objindex < scn->objects.size(); objindex++)
						{
							for (int RlobjIndex = 0; RlobjIndex < ResourceHandler::GetInstance()->RLObjects->size(); RlobjIndex++)
							{
								if (scn->objects.at(objindex) == ResourceHandler::GetInstance()->RLObjects->at(RlobjIndex).id)
								{
									RLObject* rl_obj = &ResourceHandler::GetInstance()->RLObjects->at(RlobjIndex);

									RLFetchedResource* rlResources = ResourceHandler::GetInstance()->fetchResourcesFromMesh(rl_obj->renderMeshID);
									if (rlResources)
									{
										


										//Object* newObj = new Object(this->builderCxt, )
									}
									

								}
							}
						}

					}

				}
			}
			
		}
		

	}

}



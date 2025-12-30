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
	Case* new_case = new Case(this->builderCxt,caseName_p);
	this->cases->push_back(new_case);
	this->currentCase = new_case;
	return new_case;
}

Scene* CaseHandler::CreateScene(SceneType scnType, std::string scnName)
{
	if (builderCxt->sceneExists(scnName))
	{
		ToastComponent::GetInstance()->addMessage("Scene Already exists!!");
		return nullptr;
	}
	else {
		Scene* new_scn = new Scene(this->builderCxt, scnType, scnName);
		this->currentCase->addSceneToCase(new_scn);
		return new_scn;
	}
	
	
	//return new Scene(scnType, scnName);
}

Object* CaseHandler::CreateObject(std::string objectName_p, PhysXType type)
{
	//Object* new_obj = new Object(this->builderCxt,objectName_p,type);
	//return new_obj;
}

RenderMesh* CaseHandler::CreateRenderMesh(std::string meshName_p)
{
	RenderMesh* new_msh = new RenderMesh(this->builderCxt, meshName_p);
	return new_msh;
}

ColliderMesh* CaseHandler::CreateColliderMesh(std::string MeshName_p)
{
	ColliderMesh* new_msh = new ColliderMesh(this->builderCxt, MeshName_p);
	return new_msh;
}

Shader* CaseHandler::CreateShader(Ogre::MaterialPtr mat_p, ShaderType type)
{
	Shader* new_shader = new Shader(this->builderCxt, mat_p, type);
	return new_shader;
}

Material* CaseHandler::CreateMaterial(std::string materialName_p)
{
	Material* new_mat = new Material(this->builderCxt, materialName_p);
	return new_mat;
}

Image* CaseHandler::CreateImage(std::filesystem::path filePath_p)
{
	Image* new_img = new Image(filePath_p);
	return new_img;
}

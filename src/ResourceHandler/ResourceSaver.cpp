#include "ResourceSaver.h"

bool ResourceSaver::loadIniFile(std::string filename)
{
	ini->Reset();
	SI_Error rc = ini->LoadFile(filename.c_str());
	if (rc < 0)
	{
		return false;
	}

	return true;
	
}

void ResourceSaver::saveIni(std::string filename)
{
	SI_Error rc = ini->SaveFile(filename.c_str());
	if (rc < 0)
	{
		throw ResourceHandlerIDError("Error Saving File - SaveFile() filepath invalid");
	}
	
}

void ResourceSaver::resetIni()
{
	ini->Reset();
}

void ResourceSaver::saveIniFile(std::string filename, std::string section, std::string key, std::string value)
{
	if (!loadIniFile(filename)) {
		throw ResourceHandlerIDError("Error Saving File - File not found!");
	}

	ini->SetValue(section.c_str(), key.c_str(), value.c_str());

}

void ResourceSaver::saveMasterList(std::string instanceName, std::vector<ResID>* master_p, std::string path)
{
	loadIniFile(path);

	ini->SetValue("Instances", instanceName.c_str(), NULL);

	for (int i = 0; i < master_p->size(); i++)
	{
		ini->SetValue("ID", std::to_string(master_p->at(i)).c_str(), NULL);
	}

	ini->SaveFile(path.c_str());
	ini->Reset();

}

void ResourceSaver::saveCase(CaseResource* case_p)
{
	const char* sectionName = std::to_string(case_p->getId()).c_str();

	ini->SetValue(sectionName, case_p->getName().c_str(),NULL);

	for (int i = 0; i < case_p->getScenesInCase()->size(); i++)
	{
		ini->SetValue(sectionName, std::to_string(case_p->getScenesInCase()->at(i)).c_str(), NULL);
	}

}

void ResourceSaver::saveScene(SceneResource* scene_p)
{
	const char* sectionName = std::to_string(scene_p->getId()).c_str();

	ini->SetValue(sectionName, scene_p->getName().c_str(), NULL);

	ini->SetValue(sectionName, "position", convertVec3ToString(scene_p->getPosition()).c_str());
	ini->SetValue(sectionName, "scale", convertVec3ToString(scene_p->getScale()).c_str());
	ini->SetValue(sectionName, "rotation", convertVec4ToString(scene_p->getOrientation()).c_str());

	for (int i = 0; i < scene_p->getObjects()->size(); i++)
	{
		ini->SetValue(sectionName, "Scene", std::to_string(scene_p->getObjects()->at(i)).c_str());
	}

}

void ResourceSaver::saveScnObj(std::string sectionName, ResID objectID)
{
	ini->SetValue(sectionName.c_str(), std::to_string(objectID).c_str(),NULL);
}

void ResourceSaver::saveMaterial(MaterialResource* mat_p)
{
	const char* sectionName = std::to_string(mat_p->getId()).c_str();

	ini->SetValue(sectionName, mat_p->getName().c_str(), NULL);
	ini->SetValue(sectionName, "vertexShader", std::to_string(mat_p->getVertexShader()).c_str());
	ini->SetValue(sectionName, "fragmentShader", std::to_string(mat_p->getFragmentShader()).c_str());


}

void ResourceSaver::saveMaterialTexture(std::string sectionName, ResID textureID)
{
	ini->SetValue(sectionName.c_str(), std::to_string(textureID).c_str(), NULL);
}

void ResourceSaver::saveRenderMesh(RenderMeshResource* renderMesh_p)
{
	const char* section = std::to_string(renderMesh_p->getId()).c_str();
	ini->SetValue(section, "meshName", renderMesh_p->getName().c_str());
	ini->SetValue(section, "meshFileName", renderMesh_p->getMeshFileName().c_str());
	ini->SetValue(section, "materialID", renderMesh_p->getName().c_str());

}

void ResourceSaver::saveColliderMesh(ColliderMeshResource* colliderMesh_p)
{
	const char* section = std::to_string(colliderMesh_p->getId()).c_str();
	ini->SetValue(section, "meshName", colliderMesh_p->getName().c_str());
	ini->SetValue(section, "meshFileName", colliderMesh_p->getMeshFileName().c_str());
	ini->SetValue(section, "materialID", colliderMesh_p->getName().c_str());
}

void ResourceSaver::saveShader(ShaderResource* shader_p)
{
	const char* section = std::to_string(shader_p->getId()).c_str();
	ini->SetValue(section, "shaderName", shader_p->getName().c_str());
	ini->SetValue(section, "shaderFileName", shader_p->getShaderFileName().c_str());
	ini->SetValue(section, "shaderType", std::to_string(shader_p->getShaderType()).c_str());

}

void ResourceSaver::saveShaderVar(std::string sectionName, std::string varName)
{
	ini->SetValue(sectionName.c_str(), varName.c_str(), NULL);
}

void ResourceSaver::saveShaderValue(std::string sectionName,std::string key, std::string value)
{
	ini->SetValue(sectionName.c_str(), key.c_str(), value.c_str());
}

void ResourceSaver::saveImage(ImageResource* image_p)
{
	const char* section = std::to_string(image_p->getId()).c_str();

	ini->SetValue(section, "imageName", image_p->getName().c_str());
	ini->SetValue(section, "imagePath", image_p->getImagePath().string().c_str());


}

void ResourceSaver::saveObject(ObjectResource* obj_p)
{
	const char* section = std::to_string(obj_p->getId()).c_str();

	ini->SetValue(section, "objectName", obj_p->getName().c_str());
	ini->SetValue(section, "physx", std::to_string(obj_p->getPhysxType()).c_str());
	ini->SetValue(section, "RenderMeshID", std::to_string(obj_p->getRenderMesh()).c_str());
	ini->SetValue(section, "ColliderMeshID", std::to_string(obj_p->getColliderMesh()).c_str());
	ini->SetValue(section, "mass", std::to_string(obj_p->getMass()).c_str());
}






// PUBLIC FUNCTIONS

void ResourceSaver::saveCases(std::vector<CaseResource*>* case_res, std::string caseInipath)
{
	this->resetIni();
	for (int i = 0; i < case_res->size(); i++)
	{
		
		this->saveCase(case_res->at(i));
		
	}
	this->saveIni(caseInipath);
	this->resetIni();
}

void ResourceSaver::saveScenes(std::vector<SceneResource*>* scene_res, std::string scnInipath, std::string scnObjInipath)
{
	
	
	for (int i = 0; i < scene_res->size(); i++)
	{
		this->resetIni();
		this->saveScene(scene_res->at(i));
		this->saveIni(scnInipath);
		this->resetIni();
		for (int j = 0; j < scene_res->at(i)->getObjects()->size(); j++)
		{
			this->saveScnObj(std::to_string(scene_res->at(i)->getId()), scene_res->at(i)->getObjects()->at(j));
		}
		this->saveIni(scnObjInipath);
			
	}
	this->resetIni();

}

void ResourceSaver::saveMaterials(std::vector<MaterialResource*>* mat_res, std::string matIniPath, std::string matTexIniPath)
{
	for (int i = 0; i < mat_res->size(); i++)
	{ 
		this->resetIni();
		this->saveMaterial(mat_res->at(i));
		this->saveIni(matIniPath);

		this->resetIni();
		for (int j = 0; j < mat_res->at(i)->getTextures()->size(); j++)
		{
			this->saveMaterialTexture(std::to_string(mat_res->at(i)->getId()), mat_res->at(i)->getTextures()->at(j).texture);
		}
		this->saveIni(matTexIniPath);

	}
	this->resetIni();
}

void ResourceSaver::saveObjects(std::vector<ObjectResource*>* obj_res, std::string objIniPath)
{
	this->resetIni();
	for (int i = 0; i < obj_res->size(); i++)
	{
		this->saveObject(obj_res->at(i));

	}
	this->saveIni(objIniPath);
	this->resetIni();
}

void ResourceSaver::saveColliderMesh(std::vector<ColliderMeshResource*>* col_res, std::string colIniPath)
{
	this->resetIni();
	for (int i = 0; i < col_res->size(); i++)
	{
		this->saveColliderMesh(col_res->at(i));
	}
	this->saveIni(colIniPath);
	this->resetIni();
}

void ResourceSaver::saveRenderMesh(std::vector<RenderMeshResource*>* render_res, std::string renderIniPath)
{
	this->resetIni();
	for (int i = 0; i < render_res->size(); i++)
	{
		this->saveRenderMesh(render_res->at(i));
	}
	this->saveIni(renderIniPath);
	this->resetIni();
}

void ResourceSaver::saveShaders(std::vector<ShaderResource*>* shader_res, std::string shaderIniPath, std::string shaderVarIniPath, std::string shaderValueIniPath)
{
	
	for (int i = 0; i < shader_res->size(); i++)
	{
		const char* sectionName = std::to_string(shader_res->at(i)->getId()).c_str();

		this->resetIni();
		this->saveShader(shader_res->at(i));
		this->saveIni(shaderIniPath);

		this->resetIni();
		for (int j = 0; j < shader_res->at(i)->getShaderVars()->size(); j++)
		{
			this->saveShaderVar(sectionName, shader_res->at(i)->getShaderVars()->at(j).varName);
		}
		this->saveIni(shaderVarIniPath);
		this->resetIni();
		for (int j = 0; j < shader_res->at(i)->getShaderVars()->size(); j++)
		{
			this->saveShaderValue(sectionName, shader_res->at(i)->getShaderVars()->at(j).varName,convertShaderVarValueToStr(shader_res->at(i)->getShaderVars()->at(j)));
		}
		this->saveIni(shaderValueIniPath);
		this->resetIni();
		

	}
}

void ResourceSaver::saveImages(std::vector<ImageResource*>* image_res, std::string imageIniPath)
{
	this->resetIni();
	for (int i = 0; i < image_res->size(); i++)
	{
		this->saveImage(image_res->at(i));

	}
	this->saveIni(imageIniPath);
	this->resetIni();
}

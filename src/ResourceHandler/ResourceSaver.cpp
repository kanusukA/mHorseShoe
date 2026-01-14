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

void ResourceSaver::saveMasterList(std::string instanceName, std::vector<ResID>* master_p, std::string path, bool overwrite)
{
	ini->Reset();
	if (!overwrite)
	{
		loadIniFile(path);
	}
	
	ini->SetValue(MASTER_INSTANCE_SECTION, instanceName.c_str(), NULL);

	for (int i = 0; i < master_p->size(); i++)
	{
		ini->SetValue(MASTER_INSTANCE_SECTION, std::to_string(master_p->at(i)).c_str(), NULL);
	}

	ini->SaveFile(path.c_str());
	ini->Reset();

}

void ResourceSaver::saveCase(CaseResource* case_p)
{
	std::string sectionName = std::to_string(case_p->getId());

	ini->SetValue(sectionName.c_str(), CASE_NAME_KEY , case_p->getName().c_str());

	std::string scenes = "";

	for (int i = 0; i < case_p->getScenesIdInCase()->size(); i++)
	{
		scenes += std::to_string(case_p->getScenesIdInCase()->at(i)) + "|";
	}
	ini->SetValue(sectionName.c_str(), CASE_SCENE_KEY, scenes.c_str());

}

void ResourceSaver::saveScene(SceneResource* scene_p)
{

	std::string sectionName = std::to_string(scene_p->getId());

	ini->SetValue(sectionName.c_str(), SCENE_NAME_KEY, scene_p->getName().c_str());

	ini->SetValue(sectionName.c_str(), SCENE_TYPE_KEY, std::to_string(scene_p->getSceneType()).c_str());

	// TODO FIX THIS POSITION TO STRING !!!!!!!!!!!!!!!!!!
	ini->SetValue(sectionName.c_str(), SCENE_POS_KEY, convertFloatPtrToString(scene_p->_getPosition(),3).c_str());
	ini->SetValue(sectionName.c_str(), SCENE_SCALE_KEY, convertFloatPtrToString(scene_p->_getScale(),3).c_str());
	ini->SetValue(sectionName.c_str(), SCENE_ROT_KEY, convertFloatPtrToString(scene_p->_getOrientation(),4).c_str());

	std::string scenes = "";
	std::string objects = "";

	for (int i = 0; i < scene_p->getAttachedScenesID()->size(); i++)
	{
		scenes += std::to_string(scene_p->getAttachedScenesID()->at(i)) + "|";
	}
	ini->SetValue(sectionName.c_str(), ATTACHED_SCN_KEY, scenes.c_str());
	for (int i = 0; i < scene_p->getObjectsID()->size(); i++)
	{
		objects += std::to_string(scene_p->getObjectsID()->at(i)) + "|";
	}
	ini->SetValue(sectionName.c_str(), SCENE_OBJECT_KEY, objects.c_str());

}

void ResourceSaver::saveScnObj(std::string sectionName, ResID objectID)
{
	ini->SetValue(sectionName.c_str(), std::to_string(objectID).c_str(), NULL );
}

void ResourceSaver::saveMaterial(MaterialResource* mat_p)
{
	std::string sectionName = std::to_string(mat_p->getId());

	ini->SetValue(sectionName.c_str(), MATERIAL_NAME_KEY , mat_p->getName().c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_VERTEX_KEY, std::to_string(mat_p->getVertexShaderID()).c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_FRAGMENT_KEY, std::to_string(mat_p->getFragmentShaderID()).c_str());

	std::string texValue = "";
	
	for (auto & i : *mat_p->getTextures())
	{
		std::string texId = std::to_string(i.texture);
		std::string pos = std::to_string(i.texturePosition);
		texValue += std::to_string(i.textureName.size())+";" + i.textureName + std::to_string(texId.size())
			+";" + texId + std::to_string(pos.size())+";"+ pos;
	}
	texValue += '|';
	ini->SetValue(sectionName.c_str(), MATERIAL_TEXTURE_SIZE_KEY, std::to_string(mat_p->getTextures()->size()).c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_TEXTURE_KEY, texValue.c_str());
}

void ResourceSaver::saveMaterialTexture(std::string sectionName, std::string textureName , int pos)
{
	ini->SetValue(sectionName.c_str(), std::to_string(pos).c_str(), textureName.c_str());
}

// Debug ResourceSaver functions and start ResourceLoader (Also setup MasterList.ini)

void ResourceSaver::saveRenderMesh(RenderMeshResource* renderMesh_p)
{
	std::string section = std::to_string(renderMesh_p->getId());
	ini->SetValue(section.c_str(), MESH_NAME_KEY, renderMesh_p->getName().c_str());
	// TODO FECTH LOCATION MANUALLY
	
	//ini->SetValue(section.c_str(), MESH_MATERIAL_KEY, std::to_string(renderMesh_p->getMaterialID()).c_str());

}

void ResourceSaver::saveColliderMesh(ColliderMeshResource* colliderMesh_p)
{
	const char* section = std::to_string(colliderMesh_p->getId()).c_str();
	ini->SetValue(section, MESH_NAME_KEY, colliderMesh_p->getName().c_str());
	// TODO FETCH LOCATION MANUALLY
	//ini->SetValue(section, "meshFileName", colliderMesh_p->getMeshFileName().c_str());
	ini->SetValue(section, MESH_MATERIAL_KEY, colliderMesh_p->getName().c_str());
}

void ResourceSaver::saveShader(ShaderResource* shader_p)
{
	std::string section = std::to_string(shader_p->getId());
	ini->SetValue(section.c_str(), SHADER_NAME_KEY, shader_p->getName().c_str());
	ini->SetValue(section.c_str(), SHADER_FILE_KEY, shader_p->getShaderFileName().c_str());
	ini->SetValue(section.c_str(), SHADER_TYPE_KEY, std::to_string(shader_p->getShaderType()).c_str());

}

void ResourceSaver::saveShaderVar(std::string sectionName, std::string varName, std::string type)
{
	ini->SetValue(sectionName.c_str(), varName.c_str(), type.c_str());
}

void ResourceSaver::saveShaderValue(std::string sectionName,std::string key, std::string value)
{
	ini->SetValue(sectionName.c_str(), key.c_str(), value.c_str());
}

void ResourceSaver::saveImage(ImageResource* image_p)
{
	std::string section = std::to_string(image_p->getId());

	ini->SetValue(section.c_str(), image_p->getName().c_str(), image_p->getImagePath().string().c_str());
	//ini->SetValue(section, , NULL);


}

void ResourceSaver::saveObject(ObjectResource* obj_p)
{
	std::string section = std::to_string(obj_p->getId());

	ini->SetValue(section.c_str(), OBJECT_NAME_KEY, obj_p->getName().c_str());
	ini->SetValue(section.c_str(), OBJECT_PHYSX_KEY, std::to_string(obj_p->getPhysxType()).c_str());
	ini->SetValue(section.c_str(), OBJECT_RENDERMESH_KEY, std::to_string(obj_p->getRenderMeshId()).c_str());
	ini->SetValue(section.c_str(), OBJECT_COLLIDERMESH_KEY, std::to_string(obj_p->getColliderMeshId()).c_str());
	ini->SetValue(section.c_str(), OBJECT_MASS_KEY, std::to_string(obj_p->getMass()).c_str());
}






// PUBLIC FUNCTIONS

void ResourceSaver::saveCases(std::vector<CaseResource*>* case_res, std::string caseInipath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(caseInipath);
	}
	for (int i = 0; i < case_res->size(); i++)
	{
	
		this->saveCase(case_res->at(i));
		
	}
	this->saveIni(caseInipath);
	this->resetIni();
}

void ResourceSaver::saveScenes(std::vector<SceneResource*>* scene_res, std::string scnInipath, std::string scnObjInipath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(scnInipath);
	}
	
	for (int i = 0; i < scene_res->size(); i++)
	{
		
		
		this->saveScene(scene_res->at(i));
		

		/*this->resetIni();
		this->loadIniFile(scnObjInipath);
		for (int j = 0; j < scene_res->at(i)->getObjectsID()->size(); j++)
		{
			this->saveScnObj(std::to_string(scene_res->at(i)->getId()), scene_res->at(i)->getObjectsID()->at(j));
		}
		this->saveIni(scnObjInipath);*/

	}
	this->saveIni(scnInipath);
	this->resetIni();
	

}

void ResourceSaver::saveMaterials(std::vector<MaterialResource*>* mat_res, std::string matIniPath, std::string matTexIniPath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(matIniPath);
	}
	for (int i = 0; i < mat_res->size(); i++)
	{ 
		
		this->saveMaterial(mat_res->at(i));
		
		/*this->resetIni();

		if (!overwrite || i > 0)
		{
			this->loadIniFile(matTexIniPath);
		}*/
		
		

		this->saveIni(matIniPath);
		/*this->saveIni(matTexIniPath);

		this->resetIni();
		this->loadIniFile(matIniPath);*/

	}
	this->resetIni();
}

void ResourceSaver::saveObjects(std::vector<ObjectResource*>* obj_res, std::string objIniPath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(objIniPath);
	}
	for (int i = 0; i < obj_res->size(); i++)
	{
		this->saveObject(obj_res->at(i));

	}
	this->saveIni(objIniPath);
	this->resetIni();
}

void ResourceSaver::saveColliderMesh(std::vector<ColliderMeshResource*>* col_res, std::string colIniPath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(colIniPath);
	}
	for (int i = 0; i < col_res->size(); i++)
	{
		this->saveColliderMesh(col_res->at(i));
	}
	this->saveIni(colIniPath);
	this->resetIni();
}

void ResourceSaver::saveRenderMesh(std::vector<RenderMeshResource*>* render_res, std::string renderIniPath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(renderIniPath);
	}
	for (int i = 0; i < render_res->size(); i++)
	{
		this->saveRenderMesh(render_res->at(i));
	}
	this->saveIni(renderIniPath);
	this->resetIni();
}

void ResourceSaver::saveShaders(std::vector<ShaderResource*>* shader_res, std::string shaderIniPath, std::string shaderVarIniPath, std::string shaderValueIniPath,bool overwrite)
{
	this->resetIni();

	if (!overwrite)
	{
		this->loadIniFile(shaderIniPath);
	}
	
	for (int i = 0; i < shader_res->size(); i++)
	{
		std::string sectionName = std::to_string(shader_res->at(i)->getId());

		

		this->saveShader(shader_res->at(i));
		this->saveIni(shaderIniPath);

		this->resetIni();

		if (!overwrite || i > 0)
		{
			this->loadIniFile(shaderVarIniPath);
		}

		for (int j = 0; j < shader_res->at(i)->getShaderVars()->size(); j++)
		{
			this->saveShaderVar(sectionName, shader_res->at(i)->getShaderVars()->at(j).varName, std::to_string(shader_res->at(i)->getShaderVars()->at(j).varType));
		}
		this->saveIni(shaderVarIniPath);
		this->resetIni();

		if (!overwrite || i > 0)
		{
			this->loadIniFile(shaderValueIniPath);
		}

		for (int j = 0; j < shader_res->at(i)->getShaderVars()->size(); j++)
		{
			this->saveShaderValue(sectionName, shader_res->at(i)->getShaderVars()->at(j).varName,convertShaderVarValueToStr(shader_res->at(i)->getShaderVars()->at(j)));
		}
		this->saveIni(shaderValueIniPath);
		this->resetIni();
		this->loadIniFile(shaderIniPath);

	}
	this->resetIni();
}

void ResourceSaver::saveImages(std::vector<ImageResource*>* image_res, std::string imageIniPath, bool overwrite)
{
	this->resetIni();
	if (!overwrite)
	{
		this->loadIniFile(imageIniPath);
	}
	for (int i = 0; i < image_res->size(); i++)
	{
		this->saveImage(image_res->at(i));

	}
	this->saveIni(imageIniPath);
	this->resetIni();
}

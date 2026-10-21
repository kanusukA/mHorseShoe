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

bool ResourceSaver::openSaveFile(std::string filepath)
{
	outStreamFile.open(filepath);
	if (!outStreamFile.is_open())
	{
		ToastComponent::GetInstance()->addMessage("Invalid Filepath. ResourceSaver outputFile not open.");
		return false;
	}
	return true;
}

void ResourceSaver::writeToSaveFile(const char* data)
{
	if (outStreamFile.is_open())
	{
		outStreamFile << data;
	}
}

void ResourceSaver::closeSaveFile()
{
	outStreamFile.close();
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

std::string ResourceSaver::startSaveCase(CaseResource* case_p, YAML::Emitter& out)
{
	// YAML REFORM ------------------------------------------------------------
	std::string outputFileDir = this->saveLocation + "/" + case_p->getName() + ".yml";

	if (this->openSaveFile(outputFileDir))
	{
		out << YAML::BeginMap;
		out << YAML::Key << CASE_NAME_KEY;
		out << YAML::Value << case_p->getName();
		

	}
	
	return outputFileDir;
	// -------------------------------------------------------------------------

}

void ResourceSaver::SaveCase(YAML::Emitter& out)
{
	out << YAML::EndMap;
	outStreamFile << out.c_str();
	outStreamFile.close();
}

YAML::Emitter& ResourceSaver::startSceneSave(YAML::Emitter& out)
{
	out << YAML::Key << CASE_SCENES_KEY;
	out << YAML::Value;
	out << YAML::BeginSeq;

	return out;

}

YAML::Emitter& ResourceSaver::endSceneSave(YAML::Emitter& out)
{
	out << YAML::EndSeq;
	return out;

}

YAML::Emitter& ResourceSaver::startSceneObjSave(SceneResource* scene_p, YAML::Emitter& out)
{
	out << YAML::Key << SCENE_OBJECTS_KEY;
	out << YAML::Value;
	out << YAML::BeginSeq;
	return out;

}

YAML::Emitter& ResourceSaver::endSceneObjSave(SceneResource* scene_p, YAML::Emitter& out)
{
	out << YAML::EndSeq;
	return out;
}

YAML::Emitter& ResourceSaver::saveScnObj(ObjectResource* obj, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << OBJECT_NAME_KEY;
	out << YAML::Value << obj->getName();
	out << YAML::Key << OBJECT_RENDERMESH_KEY;
	out << YAML::Value;
	return saveRenderMesh(obj->meshResource, out) 
		<< YAML::EndMap;
}

YAML::Emitter& ResourceSaver::saveRenderMesh(RenderMeshResource* mesh, YAML::Emitter& out)
{
	out << YAML::BeginMap;

	out << YAML::Key << RM_NAME;
	out << YAML::Value << mesh->getName();

	out << YAML::Key << RM_POSITION << YAML::Value;
	glm3Save(mesh->position, out);

	out << YAML::Key << RM_ROTATION << YAML::Value;
	glm3Save(mesh->rotation, out);

	out << YAML::Key << RM_SCALE << YAML::Value;
	glm3Save(mesh->scale, out);

	out << YAML::Key << RM_MESH_PATH;
	out << YAML::Value << mesh->meshFile.string().c_str();

	out << YAML::Key << RM_SHADER;
	out << YAML::Value;
	saveShader(mesh->shaderResource, out);

	out << YAML::Key << RM_SHADER_VARS;
	out << YAML::Value;
	startShaderVarSave(out);
	for (size_t i = 0; i < mesh->fragParameters.size(); i++)
	{
		shaderVarSave(mesh->fragParameters.at(i), out);
	}
	endShaderVarSave(out);

	out << YAML::EndMap;

	return out;
	
}

YAML::Emitter& ResourceSaver::saveShader(ShaderResource* shader, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << SHADER_NAME_KEY;
	out << YAML::Value << shader->getName();

	out << YAML::Key << SHADER_VERT_PATH;
	out << YAML::Value << shader->vertPath.string().c_str();

	out << YAML::Key << SHADER_FRAG_PATH;
	out << YAML::Value << shader->fragPath.string().c_str();

	out << YAML::EndMap;

	return out;

	// fetch fresh vars

}


YAML::Emitter& ResourceSaver::endSceneScnSave(YAML::Emitter& out)
{
	out << YAML::EndMap;
	return out;
}

YAML::Emitter& ResourceSaver::saveScnScn(SceneResource* scn, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << SCENE_NAME_KEY;
	out << YAML::Value << scn->getName();

	return out;

	

}

//void ResourceSaver::saveScene(SceneResource* scene_p, YAML::Emitter& out)
//{
//	
//	out << YAML::BeginMap;
//
//	out << YAML::Key << SCENE_NAME_KEY;
//	out << YAML::Value << scene_p->getName();
//	
//
//	
//}
//
//void ResourceSaver::saveScnObj(std::string sectionName, ResID objectID)
//{
//	ini->SetValue(sectionName.c_str(), std::to_string(objectID).c_str(), NULL );
//}

inline void _shaderYaml(std::vector<ShaderVar>* shadervars, YAML::Emitter& out) {
	out << YAML::Value << YAML::BeginSeq;
	for (int i = 0; i < shadervars->size(); i++)
	{
		out << YAML::BeginMap << YAML::Key << SHADER_NAME_KEY << YAML::Value << shadervars->at(i).varName;
		//out << YAML::Key << SHADER_VALUE_KEY;
		switch (shadervars->at(i).varType)
		{
		case ShaderVarType::INTEGER:
			out << YAML::Value << *shadervars->at(i).varInt;
			break;
		case ShaderVarType::FLOAT0:
			out << YAML::Value << *shadervars->at(i).varFloat;
			break;
		case ShaderVarType::FLOAT2:
			out << YAML::Value << YAML::Flow << YAML::BeginSeq << shadervars->at(i).varFloat2[0] << shadervars->at(i).varFloat2[1] << YAML::EndSeq;
			break;
		case ShaderVarType::FLOAT3:
			out << YAML::Value << YAML::Flow << YAML::BeginSeq << shadervars->at(i).varFloat3[0] << shadervars->at(i).varFloat3[1]
				<< shadervars->at(i).varFloat3[2] << YAML::EndSeq;
			break;
		case ShaderVarType::FLOAT4:
			out << YAML::Value << YAML::Flow << YAML::BeginSeq << shadervars->at(i).varFloat4[0] << shadervars->at(i).varFloat4[1]
				<< shadervars->at(i).varFloat4[2] << shadervars->at(i).varFloat4[2] << YAML::EndSeq;
			break;
		default:
			out << YAML::Value << " ";
			break;
		}
		out << YAML::Key << SHADER_TYPE_KEY;
		out << YAML::Value << shadervars->at(i).varType;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
}

void ResourceSaver::saveMaterial(MaterialResource* mat_p, ShaderResource* vert_p, ShaderResource* frag_p, YAML::Emitter& out)
{

	out << YAML::BeginMap;
	
	out << YAML::Key << MATERIAL_NAME_KEY;
	out << YAML::Value << mat_p->getName();

	out << YAML::Key << MATERIAL_WIREFRAME_KEY;
	out << YAML::Value << mat_p->wireframeMode;

	out << YAML::Key << MATERIAL_CULLING_KEY;
	//out << YAML::Value << static_cast<int>(mat_p->culling);

	out << YAML::Key << MATERIAL_FILENAME_KEY;
	out << YAML::Value << mat_p->materialFilePath;

	out << YAML::Key << MATERIAL_VERTEX_KEY;
	
	if (vert_p)
	{
		//_shaderYaml(vert_p->getShaderVars(), out);
	}
	else {
		out << YAML::Value << "";
	}

	out << YAML::Key << MATERIAL_FRAGMENT_KEY;

	if (vert_p)
	{
		//_shaderYaml(frag_p->getShaderVars(), out);
	}
	else {
		out << YAML::Value << "";
	}

	// Save Material Textures

	out << YAML::Key << MATERIAL_TEXTURE_KEY << YAML::Value << YAML::BeginSeq;

	if (mat_p->textures && mat_p->textures->size())
	{
		for (int i = 0; i < mat_p->textures->size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << MATERIAL_TEXTURE_NAME_KEY << YAML::Value << mat_p->textures->at(i).textureName;
			out << YAML::Key << MATERIAL_TEXTURE_POSITION_KEY << YAML::Value << mat_p->textures->at(i).texturePosition;
			//out << YAML::Key << MATERIAL_TEXTURE_ADDRESSING_MODE_KEY << YAML::Value << static_cast<int>(mat_p->textures->at(i).addressingMode);
			out << YAML::Key << MATERIAL_TEXTURE_SCALE_KEY << YAML::Value << mat_p->textures->at(i).scale;
			out << YAML::EndMap;
		}
	}
	out << YAML::EndSeq;

	/*std::string sectionName = std::to_string(mat_p->getId());

	ini->SetValue(sectionName.c_str(), MATERIAL_NAME_KEY , mat_p->getName().c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_ATTACHED_OBJECT, std::to_string(attachedToObject).c_str());*/
	


	/*ini->SetValue(sectionName.c_str(), MATERIAL_VERTEX_KEY, std::to_string(mat_p->getVertexShaderID()).c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_FRAGMENT_KEY, std::to_string(mat_p->getFragmentShaderID()).c_str());*/

	//std::string texValue = "";
	


	/*for (auto & i : *mat_p->getTextures())
	{
		std::string texId = std::to_string(i.texture);
		std::string pos = std::to_string(i.texturePosition);
		texValue += std::to_string(i.textureName.size())+";" + i.textureName + std::to_string(texId.size())
			+";" + texId + std::to_string(pos.size())+";"+ pos;
	}
	texValue += '|';
	ini->SetValue(sectionName.c_str(), MATERIAL_TEXTURE_SIZE_KEY, std::to_string(mat_p->getTextures()->size()).c_str());
	ini->SetValue(sectionName.c_str(), MATERIAL_TEXTURE_KEY, texValue.c_str());*/
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
	//const char* section = std::to_string(colliderMesh_p->getId()).c_str();
	//ini->SetValue(section, MESH_NAME_KEY, colliderMesh_p->getName().c_str());
	//// TODO FETCH LOCATION MANUALLY
	////ini->SetValue(section, "meshFileName", colliderMesh_p->getMeshFileName().c_str());
	//ini->SetValue(section, MESH_MATERIAL_KEY, colliderMesh_p->getName().c_str());
}

void ResourceSaver::saveShader(ShaderResource* shader_p, ShaderType type, ResID attachedToMaterial)
{
	std::string section = std::to_string(shader_p->getId());
	ini->SetValue(section.c_str(), SHADER_NAME_KEY, shader_p->getName().c_str());
	//ini->SetValue(section.c_str(), SHADER_FILE_KEY, shader_p->getShaderFileName().c_str());
	ini->SetValue(section.c_str(), SHADER_TYPE_KEY, std::to_string(shader_p->getShaderType()).c_str());

	// Making Data String 
	// Its an continuous string of shaderName, shaderVarType, and its value.
	std::string data = "";
	std::string key = "";
	//for (int vindex = 0; vindex < shader_p->getShaderVars()->size(); vindex++)
	//{
	//	/*data += std::to_string(vindex);
	//	data += "I";*/
	//	data += shader_p->getShaderVars()->at(vindex).varName;
	//	key += std::to_string(data.length()-1) + "N";
	//	key += std::to_string(shader_p->getShaderVars()->at(vindex).varType) + "F";

	//	switch (shader_p->getShaderVars()->at(vindex).varType)
	//	{
	//	case ShaderVarType::INTEGER:
	//		data += std::to_string(*shader_p->getShaderVars()->at(vindex).varInt);
	//		break;
	//	case ShaderVarType::FLOAT0:
	//		data += std::to_string(*shader_p->getShaderVars()->at(vindex).varFloat);
	//		break;
	//	case ShaderVarType::FLOAT2:
	//		data += std::to_string(shader_p->getShaderVars()->at(vindex).varFloat2[0]) +"|"+ 
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat2[1]);
	//		break;
	//	case ShaderVarType::FLOAT3:
	//		data += std::to_string(shader_p->getShaderVars()->at(vindex).varFloat2[0]) +"|"+
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat2[1]) + "|" +
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat2[2]);
	//		break;
	//	case ShaderVarType::FLOAT4:
	//		data += std::to_string(shader_p->getShaderVars()->at(vindex).varFloat3[0]) + "|" +
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat3[1]) + "|" +
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat3[2]) + "|" + 
	//			std::to_string(shader_p->getShaderVars()->at(vindex).varFloat3[3]);
	//		break;
	//	default:
	//		break;
	//	}

	//	key += std::to_string(data.length() - 1) + "V";
//	}

	//ini->SetValue(section.c_str(), SHADER_VAR_KEY, data.c_str()); // Complete this system
	//ini->SetValue(section.c_str(), SHADER_VALUE_KEY, key.c_str());
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

void ResourceSaver::saveObject(ObjectResource* obj_p, YAML::Emitter& out)
{

	/*out << YAML::BeginMap;
	
	out << YAML::Key << OBJECT_NAME_KEY;
	out << YAML::Value << obj_p->getName();

	out << YAML::Key << OBJECT_PHYSX_KEY;
	out << YAML::Value << obj_p->getPhysxType();

	out << YAML::Key << OBJECT_MASS_KEY;
	out << YAML::Value << obj_p->getMass();

	out << YAML::Key << OBJECT_RENDERMESH_KEY;
	out << YAML::Value << obj_p->_getMeshName();

	out << YAML::Key << OBJECT_MESHPATH_KEY;
	out << YAML::Value << obj_p->getMeshFilePath().string();*/

	//std::string section = std::to_string(obj_p->getId());

	//ini->SetValue(section.c_str(), OBJECT_NAME_KEY, obj_p->getName().c_str());
	//ini->SetValue(section.c_str(), OBJECT_PHYSX_KEY, std::to_string(obj_p->getPhysxType()).c_str());
	//ini->SetValue(section.c_str(), OBJECT_RENDERMESH_KEY, obj_p->_getMeshName().c_str());
	////ini->SetValue(section.c_str(), OBJECT_COLLIDERMESH_KEY, std::to_string(obj_p->getColliderMeshId()).c_str());
	//ini->SetValue(section.c_str(), OBJECT_MASS_KEY, std::to_string(obj_p->getMass()).c_str());

}

void ResourceSaver::saveResourceLoadPaths(std::vector<ResourceLoadPath>* loadPaths, std::string path)
{
	if (!loadPaths || loadPaths->size() == 0)
	{
		return;
	}

	this->openSaveFile(path);

	YAML::Emitter out;
	out << YAML::BeginSeq;
	for (int i = 0; i < loadPaths->size(); i++)
	{
		out << YAML::BeginMap;
		out << YAML::Key << LOAD_PATH_GROUP_NAME_KEY;
		out << YAML::Value << loadPaths->at(i).pathGroupName;

		out << YAML::Key << LOAD_PATH_GROUPS_KEY;
		out << YAML::Value << loadPaths->at(i).masterGroupName;

		out << YAML::Key << LOAD_PATH_EXTENSION_KEY;
		out << YAML::Value << YAML::BeginSeq;
		for (int j = 0; j < loadPaths->at(i).extensions->size(); j++)
		{
			out << loadPaths->at(i).extensions->at(j);
		}
		out << YAML::EndSeq;
		
		out << YAML::Key << LOAD_PATHS_KEY;
			out << YAML::Value << YAML::BeginSeq;
		for (int j = 0; j < loadPaths->at(i).paths->size(); j++) {
			out << loadPaths->at(i).paths->at(j);
		}
				out << YAML::EndSeq;
				out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	this->writeToSaveFile(out.c_str());
	this->outStreamFile.close();
	

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
	
		//this->startSaveCase(case_res->at(i));
		
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
		
		
		//this->saveScene(scene_res->at(i));
		

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
		
		//this->saveMaterial(mat_res->at(i));
		
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
	//	this->saveObject(obj_res->at(i));

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

		

		//this->saveShader(shader_res->at(i),);
		this->saveIni(shaderIniPath);

		this->resetIni();

		if (!overwrite || i > 0)
		{
			this->loadIniFile(shaderVarIniPath);
		}

	/*	for (int j = 0; j < shader_res->at(i)->getShaderVars()->size(); j++)
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
		}*/
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

YAML::Emitter& ResourceSaver::glm2Save(glm::vec2& vec, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << "X";
	out << YAML::Value << vec.x;
	out << YAML::Key << "Y";
	out << YAML::Value << vec.y;
	out << YAML::EndMap;
	return out;
}

YAML::Emitter& ResourceSaver::glm3Save(glm::vec3& vec, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << "X";
	out << YAML::Value << vec.x;
	out << YAML::Key << "Y";
	out << YAML::Value << vec.y;
	out << YAML::Key << "Z";
	out << YAML::Value << vec.z;
	out << YAML::EndMap;

	return out;
}

YAML::Emitter& ResourceSaver::glm4Save(glm::vec4& vec, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << "W";
	out << YAML::Value << vec.w;
	out << YAML::Key << "X";
	out << YAML::Value << vec.x;
	out << YAML::Key << "Y";
	out << YAML::Value << vec.y;
	out << YAML::Key << "Z";
	out << YAML::Value << vec.z;
	out << YAML::EndMap;

	return out;
}

YAML::Emitter& ResourceSaver::startShaderVarSave(YAML::Emitter& out)
{
	out << YAML::BeginSeq;
	return out;
}

YAML::Emitter& ResourceSaver::shaderVarSave(ShaderVar& var, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << SHADER_VAR_NAME;
	out << YAML::Value << var.varName;

	out << YAML::Key << SHADER_VAR_TYPE;
	out << YAML::Value << var.varType;

	switch (var.varType) {
	case ShaderVarType::FLOAT0:
		out << YAML::Key << "FLOAT0";
		out << YAML::Value << *var.varFloat;
		break;
	case ShaderVarType::FLOAT2:
		out << YAML::Key << "FLOAT2";
		out << YAML::Value;
		glm2Save(var.varFloat2, out);
		break;
	case ShaderVarType::FLOAT3:
		out << YAML::Key << "FLOAT3";
		out << YAML::Value;
		glm3Save(var.varFloat3, out);
		break;
	case ShaderVarType::FLOAT4:
		out << YAML::Key << "FLOAT4";
		out << YAML::Value;
		glm4Save(var.varFloat4, out);
		break;
	}

	out << YAML::EndMap;
	return out;
}

YAML::Emitter& ResourceSaver::endShaderVarSave(YAML::Emitter& out)
{
	out << YAML::EndSeq;
	return out;
}


// TEST THE NEW CASE SYSTEM
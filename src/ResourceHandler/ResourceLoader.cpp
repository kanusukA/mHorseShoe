#include "ResourceLoader.h"

void ResourceLoader::loadSavedPaths()
{
	ini->Reset();
	ini->LoadFile(this->loadLocationIniPath.c_str());

	CSimpleIniA::TNamesDepend loadPaths;
	ini->GetAllKeys("LoadPaths", loadPaths);


	for (const auto& entry : loadPaths )
	{
		try
		{
			int pos = std::stoi(entry.pItem);

			std::string value = ini->GetValue("LoadPaths", entry.pItem);

			if (!value.empty())
			{
				if (pos < 7)
				{
					load_paths->at(pos) = value;
				}
				else {
					load_paths->push_back(value);
				}
			}
			

		}
		catch (...)
		{
			std::cout << "Error loading ResourceSaved Path : " << entry.pItem << std::endl;

		}
		
			
		
		
	}

}


void ResourceLoader::saveLoadPaths()
{
	ini->Reset();

	for (int i = 0; i < this->load_paths->size(); i++)
	{
		if (!load_paths->at(i).empty())
		{
			ini->SetValue("LoadPaths",std::to_string(i).c_str(), this->load_paths->at(i).c_str());
		}
		
	}
	ini->SaveFile(this->loadLocationIniPath.c_str());

	ini->Reset();
}

void ResourceLoader::addLoadPath(ResourceLoaderEnums::ResourceLoadPaths pathType_p, std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->at(pathType_p) = path_p;
	}
	else {
		throw ResourceHandlerLoaderError("Load path SET invalid");
	}
	
}

void ResourceLoader::addLoadPath(std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->push_back(path_p);
	}
}



void ResourceLoader::loadSavedCases(std::string path_p)
{
	ini->Reset();
	ini->LoadFile(path_p.c_str());

	CSimpleIniA::TNamesDepend sections;
	ini->GetAllSections(sections);

	for (const auto& entry : sections)
	{
		RLCase new_case = RLCase();
		new_case.id = std::stoll(entry.pItem);
		new_case.name = ini->GetValue(entry.pItem, CASE_NAME_KEY);
		new_case.Scenes = std::vector<long long>();

		std::string scenes = ini->GetValue(entry.pItem, CASE_SCENE_KEY);

		std::string val = "";
		for (int i = 0; i < scenes.size(); i++)
		{
			if (scenes.at(i) == '|')
			{
				new_case.Scenes.push_back(std::stoll(val));
				val = "";
			}
			else {
				val += scenes.at(i);
			}
		}

		this->RLCases->push_back(new_case);
		
	}

}

void ResourceLoader::loadSavedScenes(std::string scene_p)
{
	ini->Reset();
	ini->LoadFile(scene_p.c_str());

	CSimpleIniA::TNamesDepend sections;

	ini->GetAllSections(sections);

	for (const auto& entry : sections)
	{
		RLScene new_scn = RLScene();
		new_scn.id = std::stoll(entry.pItem);
		new_scn.name = ini->GetValue(entry.pItem, SCENE_NAME_KEY);
		new_scn.scnType = std::stoi(ini->GetValue(entry.pItem, SCENE_TYPE_KEY));
		convertStringToFloatPtr(ini->GetValue(entry.pItem, SCENE_POS_KEY),new_scn.position);
		convertStringToFloatPtr(ini->GetValue(entry.pItem, SCENE_ROT_KEY), new_scn.rotation);
		convertStringToFloatPtr(ini->GetValue(entry.pItem, SCENE_SCALE_KEY), new_scn.scale);


		new_scn.Scenes = std::vector<long long>();
		new_scn.objects = std::vector<long long>();

		std::string strScenes = ini->GetValue(entry.pItem,ATTACHED_SCN_KEY);
		std::string strObjects = ini->GetValue(entry.pItem, SCENE_OBJECT_KEY);;

		std::string val = "";
		for (int i = 0; i < strScenes.size(); i++)
		{
			if (strScenes.at(i) == '|')
			{
				new_scn.Scenes.push_back(std::stoll(val));
				val = "";
			}
			else {
				val += strScenes.at(i);
			}
		}
		val = "";
		for (int i = 0; i < strObjects.size(); i++)
		{
			if (strObjects.at(i) == '|')
			{
				new_scn.objects.push_back(std::stoll(val));
				val = "";
			}
			else {
				val += strObjects.at(i);
			}
		}
		this->RLScenes->push_back(new_scn);
	}
}

void ResourceLoader::loadSavedObject(std::string path_p)
{
	ini->Reset();
	ini->LoadFile(path_p.c_str());

	CSimpleIniA::TNamesDepend sections;
	ini->GetAllSections(sections);

	for (const auto& entry : sections)
	{
		RLObject new_obj = RLObject();
		new_obj.id = std::stoll(entry.pItem);
		new_obj.name = ini->GetValue(entry.pItem, OBJECT_NAME_KEY);
		new_obj.mass = std::stoi(ini->GetValue(entry.pItem, OBJECT_PHYSX_KEY));
		new_obj.colliderMeshID = std::stoll(ini->GetValue(entry.pItem, OBJECT_COLLIDERMESH_KEY));
		new_obj.renderMeshID = std::stoll(ini->GetValue(entry.pItem, OBJECT_RENDERMESH_KEY));
		
		this->RLObjects->push_back(new_obj);
	}	
	

}

RLFetchedResource* ResourceLoader::_fetchedResourcesFromMesh(ResID meshID, std::vector<std::string>* resourcePaths_p)
{
	RLFetchedResource* resources = new RLFetchedResource();
	// Fetch Mesh

	RLMesh* mesh = _fetchMesh(meshID, resourcePaths_p->at(ResourcePaths::RenderMeshPath));

	RLMaterial* material;
	RLShader* shader;
	if (mesh)
	{
		resources->mesh = mesh;
		material = _fetchMaterial(mesh->materialID, resourcePaths_p->at(ResourcePaths::MaterialPath), resourcePaths_p->at(ResourcePaths::MaterialTexture));
		if (material)
		{
			resources->material = material;

			shader = _fetchShader(material->fragShaderID, resourcePaths_p->at(ResourcePaths::Shaders));

		}
		
	}
	return resources;

}

RLMesh* ResourceLoader::_fetchMesh(ResID id, std::string path_p)
{
	ini->Reset();
	ini->LoadFile(path_p.c_str());

	CSimpleIniA::TNamesDepend sections;
	ini->GetAllSections(sections);

	for (const auto& entry : sections) {
		if (std::stoll(entry.pItem) == id)
		{
			RLMesh* mesh = new RLMesh();

			mesh->name = ini->GetValue(entry.pItem, MESH_NAME_KEY);
			mesh->materialID = std::stoll(ini->GetValue(entry.pItem, MESH_NAME_KEY));
			ini->Reset();
			return mesh;

		}
	}
	ini->Reset();
}

RLMaterial* ResourceLoader::_fetchMaterial(ResID id, std::string path_p, std::string matTexPath_p)
{
	ini->Reset();
	ini->LoadFile(path_p.c_str());

	CSimpleIniA::TNamesDepend sections;
	ini->GetAllSections(sections);
	int texSize = 0;
	for (const auto& entry : sections)
	{
		if (id == std::stoll(entry.pItem))
		{
			RLMaterial* mat = new RLMaterial();
			mat->name = ini->GetValue(entry.pItem, MATERIAL_NAME_KEY);
			mat->fragShaderID = std::stoll(ini->GetValue(entry.pItem, MATERIAL_FRAGMENT_KEY));
			mat->vertShaderID = std::stoll(ini->GetValue(entry.pItem, MATERIAL_VERTEX_KEY));
			texSize = std::stoi(ini->GetValue(entry.pItem, MATERIAL_TEXTURE_SIZE_KEY));

			if (texSize > 0)
			{
				mat->textures = std::vector<ShaderTexture>(texSize);
				ini->Reset();
				ini->LoadFile(path_p.c_str());

				
				ShaderTexture tex = ShaderTexture();
				std::string texValue = ini->GetValue(entry.pItem, MATERIAL_TEXTURE_KEY);
				std::string buffer;
				int nextSize = 0;
				int pos = 0;
				int index = 0;
				
				while (index < texValue.size())
				{
					if (texValue.at(index) == '|')
					{
						break;
					}

					if (texValue.at(index) == ';')
					{
						nextSize = std::stoi(buffer);
						buffer = texValue.substr(index, nextSize);
						if (pos == 0)
						{
							tex.textureName = buffer;
							pos += 1;
							index += 1;
						}else if (pos == 1)
						{
							tex.texture = std::stoll(buffer);
							pos += 1;
							index += 1;
						}
						else if (pos == 2) {
							tex.texturePosition = std::stoi(buffer);
							pos = 0;

							mat->textures.push_back(tex);
							tex = ShaderTexture();
							index += 1;
						}
					}
					else {
						buffer += texValue.at(index);
						index += 1;
					}
				}

				
					
			}
			return mat;
			
		}
	}
	
	


	return nullptr;

}

void ResourceLoader::loadMaterialsDp(std::vector<std::filesystem::path>* output,std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::MaterialPath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::MaterialPath), extension, output, searchFolders);
		}
	}

}

void ResourceLoader::loadShadersDp(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::Shaders).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::Shaders), extension, output, searchFolders);
		}
	}
}

void ResourceLoader::loadMeshesDp(std::vector<std::filesystem::path>* output,std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::RenderMeshPath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::RenderMeshPath), extension, output, searchFolders);
		}
	}
}

//void ResourceLoader::loadColliderMesh(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
//{
//	if (clearOutput)
//	{
//		output->clear();
//	}
//
//	if (searchAllResources)
//	{
//		for (int i = 0; i < load_paths->size(); i++)
//		{
//			if (!load_paths->at(i).empty())
//			{
//				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
//			}
//		}
//	}
//	else {
//		if (!load_paths->at(ResourceLoaderEnums::ColliderMeshPath).empty())
//		{
//			fetchPathContents(load_paths->at(ResourceLoaderEnums::ColliderMeshPath), extension, output, searchFolders);
//		}
//	}
//}

void ResourceLoader::loadTexturesDp(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::MaterialTexture).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::MaterialTexture), extension, output, searchFolders);
		}
	}
}



void ResourceLoader::fetchPathContents(std::string path,std::string extension, std::vector<std::filesystem::path>* output, bool searchFolders)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (searchFolders && entry.is_directory())
		{
			fetchPathContents(entry.path().string(), extension, output, true);
		}
		else if (!entry.is_directory()) {
			std::cout << "Extension:" << entry.path().extension() << std::endl;
			if (entry.path().extension() == extension)
			{
				output->push_back(entry.path());
			}
		}
	}

}




#include "ResourceLoader.h"

void ResourceLoader::loadSavedPaths()
{
	ini->Reset();
	ini->LoadFile(this->resourceLoaderIniPath.string().c_str());

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
				if(std::filesystem::exists(value))
				{
					if (pos < 7)
					{
						load_paths->at(pos) = value;
					}
					else {
						load_paths->push_back(value);
					}
				}
				else if (std::filesystem::exists(this->sourceDir.string() + value)) {
					if (pos < 7)
					{
						load_paths->at(pos) = this->sourceDir.string() + value;
					}
					else {
						load_paths->push_back(this->sourceDir.string() + value);
					}
				}
				else {
					ToastComponent::GetInstance()->addMessage("ResourceLoad Path : " + value + "\n Does not exists.");
					continue;
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
	ini->SaveFile(this->resourceLoaderIniPath.string().c_str());

	ini->Reset();
}

void ResourceLoader::addLoadPath(ResourceLoaderEnums::ResourceLoadPaths pathType_p, std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->at(pathType_p) = path_p;
	}
	else {
		if (std::filesystem::exists(this->sourceDir.string() + path_p))
		{
			this->load_paths->at(pathType_p) = this->sourceDir.string() + path_p;
		}
		else {
			throw ResourceHandlerLoaderError("Load path SET invalid");
		}
		
	}
	
}

void ResourceLoader::addLoadPath(std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->push_back(path_p);
	}
	else if (std::filesystem::exists(this->sourceDir.string() + path_p)) {
		this->load_paths->push_back(this->sourceDir.string() + path_p);
	}
	else {
		throw ResourceHandlerLoaderError("Load path SET invalid");
	}
}



RLCase ResourceLoader::fetchCaseData(std::filesystem::path yamlFilePath)
{
	RLCase rlCase = RLCase();

	// Get Case Name
	//std::ifstream yamFile(yamlFilePath);

	YAML::Node yamDoc = YAML::LoadFile(yamlFilePath.string());

	
	rlCase.name = yamDoc[CASE_NAME_KEY].as<std::string>();

	// SCENES
	if (yamDoc[CASE_SCENES_KEY].IsSequence())
	{
		for (int i = 0; i < yamDoc[CASE_SCENES_KEY].size(); i++)
		{
			RLScene scene = RLScene();

			if (yamDoc[CASE_SCENES_KEY][i].IsMap())
			{
				scene.name = yamDoc[CASE_SCENES_KEY][i][SCENE_NAME_KEY].as<std::string>();
				
				if (yamDoc[CASE_SCENES_KEY][i][SCENE_POS_KEY].IsSequence())
				{
					scene.position = new float[3]{ yamDoc[CASE_SCENES_KEY][i][SCENE_POS_KEY][0].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_POS_KEY][1].as<float>() ,
						yamDoc[CASE_SCENES_KEY][i][SCENE_POS_KEY][2].as<float>() };
				}
				if (yamDoc[CASE_SCENES_KEY][i][SCENE_ROT_KEY].IsSequence())
				{
					scene.rotation = new float[4] { yamDoc[CASE_SCENES_KEY][i][SCENE_ROT_KEY][0].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_ROT_KEY][1].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_ROT_KEY][2].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_ROT_KEY][3].as<float>()};
				}
				if (yamDoc[CASE_SCENES_KEY][i][SCENE_SCALE_KEY].IsSequence())
				{
					scene.scale = new float[3] { yamDoc[CASE_SCENES_KEY][i][SCENE_SCALE_KEY][0].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_SCALE_KEY][1].as<float>(),
						yamDoc[CASE_SCENES_KEY][i][SCENE_SCALE_KEY][2].as<float>() };
				}

				// OBJECT
				if (yamDoc[CASE_SCENES_KEY][i][SCENE_OBJECT_KEY].IsSequence())
				{
					
					for (int objIndex = 0; objIndex < yamDoc[CASE_SCENES_KEY][i][SCENE_OBJECT_KEY].size(); objIndex++)
					{
						RLObject object = RLObject();
						YAML::Node objectNode = yamDoc[CASE_SCENES_KEY][i][SCENE_OBJECT_KEY][objIndex];
						
						object.name = objectNode[OBJECT_NAME_KEY].as<std::string>();
						object.mass = objectNode[OBJECT_MASS_KEY].as<float>();

						RLMesh mesh = RLMesh();
						mesh.name = objectNode[OBJECT_RENDERMESH_KEY].as<std::string>();
						mesh.filepath = objectNode[OBJECT_MESHPATH_KEY].as<std::string>();
						

						// MATERIAL
						if (objectNode[OBJECT_MATERIAL_KEY].IsMap())
						{
							RLMaterial material = RLMaterial();
							YAML::Node materialNode = objectNode[OBJECT_MATERIAL_KEY];

							material.name = materialNode[MATERIAL_NAME_KEY].as<std::string>();
							material.materialFilePath = materialNode[MATERIAL_FILENAME_KEY].as<std::string>();
							material.culling = Ogre::CullingMode(materialNode[MATERIAL_CULLING_KEY].as<int>());
							material.wireframe = materialNode[MATERIAL_WIREFRAME_KEY].as<bool>();

							if (materialNode[MATERIAL_VERTEX_KEY].IsSequence())
							{
								RLShader shaderVert = RLShader();
								RLShader shaderFrag = RLShader();
								for (int shaderIndex = 0; shaderIndex < materialNode[MATERIAL_VERTEX_KEY].size(); shaderIndex++)
								{
									ShaderVar var = ShaderVar();
									YAML::Node shader = materialNode[MATERIAL_VERTEX_KEY][shaderIndex];

									var.varName = shader[SHADER_NAME_KEY].as<std::string>();
									var.varType = ShaderVarType(shader[SHADER_TYPE_KEY].as<int>());

									switch (var.varType)
									{
									case ShaderVarType::INTEGER:
										*var.varInt = shader[SHADER_VALUE_KEY].as<int>();
										break;
									case ShaderVarType::FLOAT0:
										*var.varFloat = shader[SHADER_VALUE_KEY].as<float>();
										break;
									case ShaderVarType::FLOAT2:
										var.varFloat2[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat2[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										break;
									case ShaderVarType::FLOAT3:
										var.varFloat3[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat3[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										var.varFloat3[2] = shader[SHADER_VALUE_KEY][2].as<float>();
										break;
									case ShaderVarType::FLOAT4:
										var.varFloat4[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat4[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										var.varFloat4[2] = shader[SHADER_VALUE_KEY][2].as<float>();
										var.varFloat4[3] = shader[SHADER_VALUE_KEY][3].as<float>();
										break;
									default:
										ToastComponent::GetInstance()->addMessage("Error loading Shader var : " + var.varName);
										break;
									}
									
									shaderVert.shaderVars.push_back(var);
									
								}
								material.vertShader = shaderVert;
							}
							if (materialNode[MATERIAL_FRAGMENT_KEY].IsSequence())
							{
								RLShader shaderFrag = RLShader();
								for (int shaderIndex = 0; shaderIndex < materialNode[MATERIAL_FRAGMENT_KEY].size(); shaderIndex++)
								{
									ShaderVar var = ShaderVar();
									YAML::Node shader = materialNode[MATERIAL_FRAGMENT_KEY][shaderIndex];

									var.varName = shader[SHADER_NAME_KEY].as<std::string>();
									var.varType = ShaderVarType(shader[SHADER_TYPE_KEY].as<int>());

									switch (var.varType)
									{
									case ShaderVarType::INTEGER:
										*var.varInt = shader[SHADER_VALUE_KEY].as<int>();
										break;
									case ShaderVarType::FLOAT0:
										*var.varFloat = shader[SHADER_VALUE_KEY].as<float>();
										break;
									case ShaderVarType::FLOAT2:
										var.varFloat2[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat2[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										break;
									case ShaderVarType::FLOAT3:
										var.varFloat3[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat3[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										var.varFloat3[2] = shader[SHADER_VALUE_KEY][2].as<float>();
										break;
									case ShaderVarType::FLOAT4:
										var.varFloat4[0] = shader[SHADER_VALUE_KEY][0].as<float>();
										var.varFloat4[1] = shader[SHADER_VALUE_KEY][1].as<float>();
										var.varFloat4[2] = shader[SHADER_VALUE_KEY][2].as<float>();
										var.varFloat4[3] = shader[SHADER_VALUE_KEY][3].as<float>();
										break;
									default:
										ToastComponent::GetInstance()->addMessage("Error loading Shader var : " + var.varName);
										break;
									}

									shaderFrag.shaderVars.push_back(var);
								}
								material.fragShader = shaderFrag;
							}
						
							object.material = material;

						}
						object.mesh = mesh;
						scene.objects.push_back(object);
					}

					
				}
				rlCase.Scenes.push_back(scene);

			}

		}
	}
	else {
		ToastComponent::GetInstance()->addMessage("SCENES not found for " + yamlFilePath.filename().string());
	}

	return rlCase;
}

//void ResourceLoader::loadSavedCases(std::string path_p)
//{
//	ini->Reset();
//	ini->LoadFile(path_p.c_str());
//
//	CSimpleIniA::TNamesDepend sections;
//	ini->GetAllSections(sections);
//
//	for (const auto& entry : sections)
//	{
//		RLCase new_case = RLCase();
//		new_case.id = std::stoll(entry.pItem);
//		new_case.name = ini->GetValue(entry.pItem, CASE_NAME_KEY);
//		//new_case.Scenes = std::vector<long long>();
//
//		std::string scenes = ini->GetValue(entry.pItem, CASE_SCENE_KEY);
//
//		std::string val = "";
//		for (int i = 0; i < scenes.size(); i++)
//		{
//			if (scenes.at(i) == '|')
//			{
//				//new_case.Scenes.push_back(std::stoll(val));
//				val = "";
//			}
//			else {
//				val += scenes.at(i);
//			}
//		}
//
//		this->RLCases->push_back(new_case);
//		
//	}
//
//}

void ResourceLoader::loadSavedCases()
{
	
	fetchPathContents(dataDirPath.string(), ".yml", &caseFileNames);
	
}

void ResourceLoader::loadSavedScenes(std::string scene_p)
{
	/*ini->Reset();
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
		this->RLScenes->push_back(new_scn);*/
//	}
}

void ResourceLoader::loadSavedObject(std::string path_p)
{
	/*ini->Reset();
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
	*/

}

RLFetchedResource* ResourceLoader::_fetchedResourcesFromMesh(ResID meshID, std::vector<std::string>* resourcePaths_p)
{
	//RLFetchedResource* resources = new RLFetchedResource();
	//// Fetch Mesh

	//RLMesh* mesh = _fetchMesh(meshID, resourcePaths_p->at(ResourcePaths::RenderMeshPath));

	//RLMaterial* material;
	//RLShader* shader;
	//if (mesh)
	//{
	//	resources->mesh = mesh;
	//	material = _fetchMaterial(mesh->materialID, resourcePaths_p->at(ResourcePaths::MaterialPath), resourcePaths_p->at(ResourcePaths::MaterialTexture));
	//	if (material)
	//	{
	//		resources->material = material;

	//		shader = _fetchShader(material->fragShaderID, resourcePaths_p->at(ResourcePaths::Shaders));

	//	}
	//	
	//}
	//return resources;
	return nullptr;

}

RLMesh* ResourceLoader::_fetchMesh(ResID id, std::string path_p)
{
	/*ini->Reset();
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
	ini->Reset();*/
	return nullptr;
}

RLMaterial* ResourceLoader::_fetchMaterial(ResID id, std::string path_p, std::string matTexPath_p)
{
	/*ini->Reset();
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
	
	


	return nullptr;*/
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
		if (!load_paths->at(ResourceLoaderEnums::ImagePath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::ImagePath), extension, output, searchFolders);
		}
	}
}



void ResourceLoader::fetchPathContents(std::string path,std::string extension, std::vector<std::filesystem::path>* output, bool searchFolders)
{
	if(std::filesystem::exists(path)) {
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
					if (!vectorPathContains(output,entry.path().filename().string()))
					{
						output->push_back(entry.path());
					}
					
				}
			}
		}
	}
	else {
		ToastComponent::GetInstance()->addMessage("Invalid path provided : " + path);
	}

}




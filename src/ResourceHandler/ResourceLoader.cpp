#include "ResourceLoader.h"

void ResourceLoader::loadSavedPaths()
{
	
	YAML::Node loadPathsNode = YAML::LoadFile(this->resourceLoaderIniPath.string());
	if (loadPathsNode.IsSequence())
	{
		for (size_t i = 0; i < loadPathsNode.size(); i++)
		{
			ResourceLoadPath path = ResourceLoadPath();

			path.pathGroupName = loadPathsNode[i][LOAD_PATH_GROUP_NAME_KEY].as<std::string>();
			path.masterGroupName = loadPathsNode[i][LOAD_PATH_GROUPS_KEY].as<std::string>();
			if (loadPathsNode[i][LOAD_PATH_EXTENSION_KEY].IsSequence())
			{
				path.extensions = new std::vector<std::string>();
				for (size_t extIndex = 0; extIndex < loadPathsNode[i][LOAD_PATH_EXTENSION_KEY].size(); extIndex++)
				{
					path.extensions->push_back(loadPathsNode[i][LOAD_PATH_EXTENSION_KEY][extIndex].as<std::string>());
				}
			}
			if (loadPathsNode[i][LOAD_PATHS_KEY].IsSequence())
			{
				path.paths = new std::vector<std::string>();
				for (size_t pathIndex = 0; pathIndex < loadPathsNode[i][LOAD_PATHS_KEY].size(); pathIndex++)
				{
					path.paths->push_back(loadPathsNode[i][LOAD_PATHS_KEY][pathIndex].as<std::string>());
				}
			}

			load_paths->push_back(path);

		}
	}

}


ResourceLoadPath* ResourceLoader::getLoadPath(std::string groupName)
{
	for (size_t i = 0; i < load_paths->size(); i++)
	{
		if (load_paths->at(i).masterGroupName == groupName)
		{
			return &load_paths->at(i);
		}
	}
	return nullptr;
}

void ResourceLoader::addLoadPath(ResourceLoadPath path_p)
{
	
	load_paths->push_back(path_p);
	
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
//							material.culling = Ogre::CullingMode(materialNode[MATERIAL_CULLING_KEY].as<int>());
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



void ResourceLoader::loadSavedCases()
{
	
	fetchPathContents(dataDirPath.string(), ".yml", &caseFileNames);
	
}



bool ResourceLoader::openLoadFile(std::string filepath)
{
	if (std::filesystem::exists(filepath))
	{
		loadFile = YAML::LoadFile(filepath);
		return true;

	}
	return false;
	
}

std::string ResourceLoader::fetchNodeByMapKey(std::string key)
{
	if (loadFile)
	{
		return loadFile[key].as<std::string>();
	}
	return "";
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




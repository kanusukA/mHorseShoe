#include "ResourceHandler.h"


// Multi-Thread shit
ResourceHandler* ResourceHandler::pinstance_{ nullptr };
std::mutex ResourceHandler::mutex_;






ResourceHandler::ResourceHandler()
{
	ToastComponent::GetInstance()->addMessage("Starting Resource Handler");
	this->SourceDir = getSourceDir();
	ToastComponent::GetInstance()->addMessage("Source Dir : " + this->SourceDir.string());

	// Load contents in load_path
	this->initResourceLoader(&ini, SourceDir, SourceDir.string() + DATA_DIRECTORY, this->SourceDir.string() + RESOURCELOADER_DATA);

	/*std::vector<ShaderVar> shaders{};
	this->readGLSLShaderFile("C:/Users/lenovo/source/repos/mHorseShoeeVCmake/mHorseShoe/required/assets/meshes/Materials/newVulkanShaders/sky_frag.slang", &shaders);*/

	// SETUP MASTER RESOURCE VECTOR
	this->setMasterLoadPaths();
	
	this->ini.SetUnicode();
	this->initResourceSaver(&ini, this->SourceDir.string() + DATA_DIRECTORY);

}


std::filesystem::path ResourceHandler::find(std::string fileName, std::string location)
{

	if (location.empty()) {
		throw ResourceHandlerInvalidRequest();
	}
	
	try {
		for (const auto& entry : fs::directory_iterator(location)) {

			if (entry.path().filename().string() == fileName) {
				std::cout << entry.path().filename() << " - found.";
				return entry.path();
			}
		}
	}
	catch (const std::exception& e) {
		throw ResourceHandlerFileNotFound();
	}
		
}


ResourceHandler* ResourceHandler::GetInstance() {

	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new ResourceHandler();
		pinstance_->ini.SetUnicode();

	}
	return pinstance_;


}



void ResourceHandler::saveLoadPaths() {
	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();
	this->saveResourceLoadPaths(loadPaths, this->SourceDir.string() + RESOURCELOADER_DATA);
}

void ResourceHandler::setMasterLoadPaths()
{
	

	masterResourceVector->clear();
	for (size_t i = 0; i < ResourceGroup::ResourceMasterGroups.size(); i++)
	{
		masterResourceVector->push_back(new ResourceMasterGroup(ResourceGroup::ResourceMasterGroups.at(i)));
	}

	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();

	if (!loadPaths || loadPaths->empty())
	{
		ToastComponent::GetInstance()->addMessage("No Load Paths to set in Master Resource Vector");
		return;
	}

	for (size_t i = 0; i < loadPaths->size(); i++)
	{
		
		for (size_t j = 0; j < masterResourceVector->size(); j++)
		{
			if (loadPaths->at(i).masterGroupName == masterResourceVector->at(j)->GroupName)
			{
				masterResourceVector->at(j)->loadPath = loadPaths->at(i).pathGroupName;
				masterResourceVector->at(j)->loadPathIndex = i;
			}
		}
	}

	syncMasterLoadPaths();


}

// Syncs the load_paths internal path! If load_path vector itself is changed then run setMasterLoadPath()!
void ResourceHandler::syncMasterLoadPaths()
{
	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();
	for (size_t i = 0; i < masterResourceVector->size(); i++)
	{
		if (masterResourceVector->at(i)->loadPath.empty())
		{
			continue;
		}
		if (masterResourceVector->at(i)->loadPathIndex < loadPaths->size() && masterResourceVector->at(i)->loadPath == loadPaths->at(masterResourceVector->at(i)->loadPathIndex).pathGroupName)
		{
			for (size_t pathsIndex = 0; pathsIndex < loadPaths->at(masterResourceVector->at(i)->loadPathIndex).paths->size(); pathsIndex++)
			{
				for (size_t extIndex = 0; extIndex < loadPaths->at(masterResourceVector->at(i)->loadPathIndex).extensions->size(); extIndex++)
				{
					fetchPathContents(loadPaths->at(masterResourceVector->at(i)->loadPathIndex).paths->at(pathsIndex), 
						loadPaths->at(masterResourceVector->at(i)->loadPathIndex).extensions->at(extIndex), masterResourceVector->at(i)->ResourcePaths,true);
				}
			}
		}
	}


}



std::vector<std::filesystem::path>* ResourceHandler::fetchResourceGroupVecByName(std::string groupName)
{
	for (size_t i = 0; i < masterResourceVector->size(); i++)
	{
		if (masterResourceVector->at(i)->GroupName == groupName)
		{
			return masterResourceVector->at(i)->ResourcePaths;
		}
	}
	return nullptr;
}

std::filesystem::path* ResourceHandler::fetchFileInGroup(std::string filename_p, std::string group_p)
{
	std::vector<std::filesystem::path>* groupVec = this->fetchResourceGroupVecByName(group_p);
	if (groupVec)
	{
		for (size_t i = 0; i < groupVec->size(); i++)
		{
			if (groupVec->at(i).filename().string() == filename_p || groupVec->at(i).stem().string() == filename_p)
			{
				return &groupVec->at(i);
			}
		}
	}
	return nullptr;
}

std::filesystem::path* ResourceHandler::fetchFileInGroup(std::string filename_p, int masterIndex)
{
	std::vector<std::filesystem::path>* groupVec = this->fetchResourceGroupVecByIndex(masterIndex);
	if (groupVec)
	{
		for (size_t i = 0; i < groupVec->size(); i++)
		{
			if (groupVec->at(i).filename().string() == filename_p || groupVec->at(i).stem().string() == filename_p)
			{
				return &groupVec->at(i);
			}
		}
	}
	return nullptr;
}

CaseResource ResourceHandler::readCaseFile(std::filesystem::path& filepath)
{
	if (std::filesystem::exists(filepath))
	{
		// Case
		YAML::Node node = YAML::LoadFile(filepath.string().c_str());

		std::string caseName = node[CASE_NAME_KEY].as<std::string>();

		std::vector<SceneResource> scenes{};

		if (node[CASE_SCENES_KEY].IsSequence())
		{
			for (size_t i = 0; i < node[CASE_SCENES_KEY].size(); i++)
			{
				YAML::Node scene = node[CASE_SCENES_KEY][i];
				SceneResource scnRes = ResourceHandler::readSceneNode(scene);
				scenes.push_back(std::move(scnRes));

			}
		}

		CaseResource caseResource = CaseResource(this, caseName, filepath.filename().string());
		caseResource._scenes = std::move(scenes);
		return caseResource;
	}

	throw std::runtime_error("FILE DOES NOT EXISTS");

	
}

SceneResource ResourceHandler::readSceneNode(YAML::Node& node)
{
	std::string scnName = node[SCENE_NAME_KEY].as<std::string>();

	SceneResource sceneResource = SceneResource(this, scnName);

	// has scenes
	if (node[CASE_SCENES_KEY].IsSequence())
	{
		for (size_t i = 0; i < node[CASE_SCENES_KEY].size(); i++)
		{
			YAML::Node scene = node[CASE_SCENES_KEY][i];
			SceneResource scnRes = ResourceHandler::readSceneNode(scene);
			sceneResource._scenes.push_back(std::move(scnRes));

		}
		
	}
	// has objects
	if (node[SCENE_OBJECTS_KEY].IsSequence())
	{
		for (size_t i = 0; i < node[SCENE_OBJECTS_KEY].size(); i++)
		{
			YAML::Node obj = node[SCENE_OBJECTS_KEY][i];
			ObjectResource objRes = ResourceHandler::readObjectNode(obj);
			sceneResource._objects.push_back(std::move(objRes));

		}
	}

	return sceneResource;

}

ObjectResource ResourceHandler::readObjectNode(YAML::Node& node)
{
	std::string objName = node[OBJECT_NAME_KEY].as<std::string>();
	YAML::Node meshNode = node[OBJECT_RENDERMESH_KEY];
	RenderMeshResource* meshRes = ResourceHandler::readMeshNode(meshNode); // TODO GRABAGE COLLECT HERE
	ObjectResource objRes = ObjectResource(this, objName, std::move(meshRes));

	return objRes;


}

RenderMeshResource* ResourceHandler::readMeshNode(YAML::Node& node)
{
	std::string meshName = node[RM_NAME].as<std::string>();
	YAML::Node positionNode = node[RM_POSITION];
	glm::vec3 position = readVec3(positionNode);
	YAML::Node rotationNode = node[RM_ROTATION];
	glm::vec3 rotation = readVec3(rotationNode);
	YAML::Node scaleNode = node[RM_SCALE];
	glm::vec3 scale = readVec3(scaleNode);

	std::string path = node[RM_MESH_PATH].as<std::string>();

	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("MESH DOES NOT EXISTS");
	}
	YAML::Node shaderNode = node[RM_SHADER];

	ShaderResource* shaderRes = ResourceHandler::readShaderNode(shaderNode);
	RenderMeshResource* meshRes = new RenderMeshResource(this,meshName,std::filesystem::path(path),std::move(shaderRes));
	YAML::Node shaderVarNode = node[RM_SHADER_VARS];
	meshRes->fragParameters = readShaderVarsNode(shaderVarNode);

	return meshRes;
}

ShaderResource* ResourceHandler::readShaderNode(YAML::Node& node)
{
	std::string shaderName = node[SHADER_NAME_KEY].as<std::string>();
	std::string vertPath = node[SHADER_VERT_PATH].as<std::string>();
	std::string fragPath = node[SHADER_FRAG_PATH].as<std::string>();

	if (!std::filesystem::exists(vertPath) || !std::filesystem::exists(fragPath))
	{
		throw std::runtime_error("SHADER PATHS NOT FOUND!");
	}

	return new ShaderResource(this, shaderName, ShaderType::Fragment, std::filesystem::path(vertPath), std::filesystem::path(fragPath));
	
}

std::vector<std::filesystem::path>* ResourceHandler::fetchResourceGroupVecByIndex(int masterIndex)
{
	if (masterResourceVector && masterIndex < masterResourceVector->size())
	{
		return masterResourceVector->at(masterIndex)->ResourcePaths;
	}
	return nullptr;
}

std::filesystem::path ResourceHandler::getSourceDir()
{
	
	WCHAR path[MAX_PATH];

	GetModuleFileNameW(NULL, path, MAX_PATH);

	char strPath[MAX_PATH];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, path, -1, strPath, MAX_PATH, &DefChar, NULL);
	std::filesystem::path srcPath = strPath;
	
	return srcPath.remove_filename();

}










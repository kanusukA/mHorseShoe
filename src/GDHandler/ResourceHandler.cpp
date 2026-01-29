#include "ResourceHandler.h"


// Multi-Thread shit
ResourceHandler* ResourceHandler::pinstance_{ nullptr };
std::mutex ResourceHandler::mutex_;

// Insert command  implementation to execute read , write and find requests

bool vectorContains(std::string term, std::vector<std::string>* vec);
bool vectorContains(std::string term, std::vector<std::filesystem::path>* vec);

ResourceHandlerType ResourceHandler::_getResourceLocationGroup(std::string groupStr)
{
	if (groupStr == RENDER_MESH_LOC) {
		return ResourceHandlerType::RENDER_MESH;
	}
	else if (groupStr == COLLIDER_MESH_LOC) {
		return ResourceHandlerType::COLLIDER_MESH;
	}
	else if (groupStr == MESH_MATERIAL_LOC) {
		return ResourceHandlerType::MESH_MATERIALS;
	}
	else if (groupStr == IMAGES_LOC)
	{
		return ResourceHandlerType::IMAGE;
	}
	else {
		return ResourceHandlerType::GLOBAL;
	}

}

std::vector<std::filesystem::path>* ResourceHandler::fetchResourcesByEnum(ResourceLoaderEnums::ResourceLoadPaths group_p)
{
	switch (group_p)
	{
	case ResourceLoaderEnums::Objects:
		return nullptr;
		break;
	case ResourceLoaderEnums::Shaders:
		return this->ShaderDp;
		break;
	case ResourceLoaderEnums::MaterialPath:
		return this->MaterialDp;
		break;
	case ResourceLoaderEnums::MaterialTexture:
		return this->TextureDp;
		break;
	case ResourceLoaderEnums::RenderMeshPath:
		return this->MeshDp;
		break;
	case ResourceLoaderEnums::ImagePath:
		return this->TextureDp;
		break;
	default:
		return nullptr;
		break;
	}
}




void ResourceHandler::_readShaderFile(std::vector<std::string>* shaderVar, std::filesystem::path path)
{

	//in_stream.open(path);
	//std::string line;

	//std::string word = "";

	//bool uniFound = false;
	//bool uniName = false;
	//bool skip = false;
	//int type;

	//std::cout << "Variables : " << std::endl;

	//if (in_stream.is_open()) {
	//	while (std::getline(in_stream, line))
	//	{
	//		//std::cout << line << std::endl;
	//		if (line == "//SKIP")
	//		{
	//			skip = true;
	//		}

	//		//std::cout << "Line : " << line << std::endl;

	//		if (skip)
	//		{
	//			if (line == "//!SKIP") {
	//				skip = false;
	//				//shaderVar->push_back(word);
	//			}

	//			continue;
	//		}

	//		// first check uniform keyword
	//		for (int i = 0; i < line.size(); i++)
	//		{
	//		

	//			if (line.at(i) != ' ' && line.at(i) != ',' && line.at(i) != ')') {
	//				word += line.at(i);
	//			}

	//			else {
	//				// skips the coming constants

	//				if (word == "uniform") {

	//					//std::cout << "uniform" << std::endl;
	//					uniFound = true;

	//				}
	//				else if (uniFound) {
	//					//std::cout << "type : ";

	//					uniFound = false;
	//					uniName = true;

	//					if (word == "int" || word == "bool") {
	//						shaderVar->push_back("0");
	//						std::cout << word << std::endl;
	//					}
	//					else if (word == "float")
	//					{
	//						shaderVar->push_back("1");
	//						std::cout << "float" << std::endl;
	//					}
	//					else if (word == "float2")
	//					{
	//						shaderVar->push_back("2");
	//						std::cout << "float2" << std::endl;
	//					}
	//					else if (word == "float3")
	//					{
	//						shaderVar->push_back("3");
	//						std::cout << "float3" << std::endl;
	//					}
	//					else if (word == "float4")
	//					{
	//						shaderVar->push_back("4");
	//						std::cout << "float4" << std::endl;
	//					}
	//					else {
	//						std::cout << "invalid type" << std::endl;
	//						uniName = false;
	//					}


	//				}
	//				else if (uniName)
	//				{
	//					shaderVar->push_back(word);
	//					//std::cout << "Word: " << word << std::endl;
	//					uniName = false;
	//				}
	//				word = "";
	//			}
	//		}

	//	}
	//}

	//in_stream.close();


}

void ResourceHandler::_LoadIniFile(std::string filename)
{
	ini.Reset();
	SI_Error rc = ini.LoadFile(filename.c_str());
	if (rc < 0)
	{
		ini.SaveFile(filename.c_str());
		ini.LoadFile(filename.c_str());
	}
}


ResourceHandler::ResourceHandler()
{
	ToastComponent::GetInstance()->addMessage("Starting Resource Handler");
	this->SourceDir = getSourceDir();

	checkFileStructure();
	
	this->ini.SetUnicode();
	this->initResourceSaver(&ini, this->SourceDir.string() + DATA_DIRECTORY);

	this->initResourceLoader(&ini, SourceDir.string() + DATA_DIRECTORY , this->SourceDir.string() + RESOURCELOADER_DATA);

	this->loadResources();

}

void ResourceHandler::checkFileStructure()
{
	ToastComponent::GetInstance()->addMessage("Checking Resource Handler File Structure");
	// Check if Data Directory Exists First
	if (!std::filesystem::exists(this->SourceDir.string() + DATA_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Data Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + DATA_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + MASTERLIST_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Master list does not exists. Creating");
		_LoadIniFile(this->SourceDir.string() + MASTERLIST_LOC);
	}
	this->setPath(this->SourceDir.string() + MASTERLIST_LOC, ResourcePaths::MasterList);

	if (!std::filesystem::exists(this->SourceDir.string() + RESOURCELOADER_DATA))
	{
		ToastComponent::GetInstance()->addMessage("Master list does not exists. Creating");
		_LoadIniFile(this->SourceDir.string() + RESOURCELOADER_DATA);
	}
	

	if (!std::filesystem::exists(this->SourceDir.string() + CASE_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Case Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + CASE_DIRECTORY);
	}


	if (!std::filesystem::exists(this->SourceDir.string() + CASE_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Cases.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + CASE_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + CASE_INI_LOC, ResourcePaths::Cases);

	if (!std::filesystem::exists(this->SourceDir.string() + SCENE_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Scene Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + SCENE_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + SCENEOBJ_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Scene Obj Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + SCENEOBJ_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + SCENE_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Scenes.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + SCENE_INI_LOC);
	}

	
	this->setPath(this->SourceDir.string() + SCENE_INI_LOC, ResourcePaths::Scenes);

	if (!std::filesystem::exists(this->SourceDir.string() + SCENEOBJ_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("ScenesObj.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + SCENEOBJ_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + SCENEOBJ_INI_LOC, ResourcePaths::ScnObj);

	if (!std::filesystem::exists(this->SourceDir.string() + MATERIAL_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Material Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + MATERIAL_DIRECTORY);
	}
	if (!std::filesystem::exists(this->SourceDir.string() + MATERIALTEXTURE_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("MaterialTexture Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + MATERIALTEXTURE_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + MATERIAL_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Materials.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + MATERIAL_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + MATERIAL_INI_LOC, ResourcePaths::MaterialPath);

	if (!std::filesystem::exists(this->SourceDir.string() + MATERIALTEXTURE_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("MaterialTextures.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + MATERIALTEXTURE_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + MATERIALTEXTURE_INI_LOC, ResourcePaths::MaterialTexture);

	if (!std::filesystem::exists(this->SourceDir.string() + RENDERMESH_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("RenderMesh Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + RENDERMESH_DIRECTORY);
	}
	if (!std::filesystem::exists(this->SourceDir.string() + RENDERMESH_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("RenderMeshes.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + RENDERMESH_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + RENDERMESH_INI_LOC, ResourcePaths::RenderMeshPath);


	if (!std::filesystem::exists(this->SourceDir.string() + SHADER_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Shader Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + SHADER_DIRECTORY);
	}
	if (!std::filesystem::exists(this->SourceDir.string() + SHADERVAR_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("ShaderVar Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + SHADERVAR_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + SHADER_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Shaders.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + SHADER_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + SHADER_INI_LOC, ResourcePaths::Shaders);


	if (!std::filesystem::exists(this->SourceDir.string() + SHADERVARS_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("ShaderVars.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + SHADERVARS_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + SHADERVARS_INI_LOC, ResourcePaths::ShaderVars);

	if (!std::filesystem::exists(this->SourceDir.string() + SHADERVALUES_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("ShaderValues.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + SHADERVALUES_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + SHADERVALUES_INI_LOC, ResourcePaths::ShaderValues);

	if (!std::filesystem::exists(this->SourceDir.string() + IMAGE_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Image Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + IMAGE_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + IMAGE_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Images.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + IMAGE_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + IMAGE_INI_LOC, ResourcePaths::ImagePath);

	if (!std::filesystem::exists(this->SourceDir.string() + OBJECT_DIRECTORY))
	{
		ToastComponent::GetInstance()->addMessage("Object Directory does not exists. Creating");
		std::filesystem::create_directory(this->SourceDir.string() + OBJECT_DIRECTORY);
	}

	if (!std::filesystem::exists(this->SourceDir.string() + OBJECT_INI_LOC))
	{
		ToastComponent::GetInstance()->addMessage("Objects.ini does not exsits. Creating");
		_LoadIniFile(this->SourceDir.string() + OBJECT_INI_LOC);
	}

	this->setPath(this->SourceDir.string() + OBJECT_INI_LOC, ResourcePaths::Objects);

}

void ResourceHandler::loadResources()
{
	this->loadMaterialsDp(this->MaterialDp, ".material", false, true);
	this->loadShadersDp(this->ShaderDp, ".hlsl", true, true);
	this->loadMeshesDp(this->MeshDp);
	this->loadTexturesDp(this->TextureDp, ".png");

	// loading saved data
	//this->loadSavedCases(*this->getPath(ResourcePaths::Cases));
	//this->loadSavedScenes(*this->getPath(ResourcePaths::Scenes));


}

RLFetchedResource* ResourceHandler::fetchResourcesFromMesh(ResID meshID)
{
	_fetchedResourcesFromMesh(meshID, allResourceParentPaths);
}


void ResourceHandler::saveResources()
{
 // TODO IMPLEMENT SAVE FUNCTION!!!!!!!!

}

std::filesystem::path ResourceHandler::fetchLocByFileName(std::string filename_p, ResourceLoaderEnums::ResourceLoadPaths group_p)
{
	std::vector<std::filesystem::path>* group = fetchResourcesByEnum(group_p);

	if (group){
		for (int i = 0; i < group->size(); i++)
		{
			if (group->at(i).filename().string() == filename_p)
			{
				return group->at(i);
			}
		}
		ToastComponent::GetInstance()->addMessage("No such filename exists");
	}
	else {
		ToastComponent::GetInstance()->addMessage("Unable to find group");
	}

}

void ResourceHandler::findAll(std::string location, ResourceHandlerType type = ResourceHandlerType::GLOBAL) {

	if (location.empty()) {
		throw ResourceHandlerInvalidRequest();
	}

	try {
		for (const auto& entry : fs::directory_iterator(location)) {
			//std::cout << "Adding resource : " << entry.path().string() << std::endl;
			//this->addResource(entry.path().string(),type);
		}
	}
	catch (const std::exception& e) {
		throw ResourceHandlerFileNotFound();
	}
}

std::filesystem::path ResourceHandler::find(std::string fileName, std::string location)
{
	//std::cout << "Opening file " << fileName << std::endl;

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


std::filesystem::path ResourceHandler::findAllInLocation(std::string filename, ResourceHandlerType type = ResourceHandlerType::GLOBAL)
{
	//in_stream.open(RESOURCE_DEFAULT_LOC);
	//std::string line;

	//if (in_stream.is_open()) {

	//	// Searching in Default Resource Loc
	//	while (std::getline(in_stream, line)) {
	//		try {
	//			std::filesystem::path path = this->find(filename, line);
	//			in_stream.close();
	//			return path;
	//		}
	//		catch (const std::exception&) {
	//			in_stream.close();
	//			throw ResourceHandlerFileNotFound();
	//		}
	//		
	//	}
	//	in_stream.close();
	//	throw ResourceHandlerFileNotFound();
	//	
	//}

}

std::string ResourceHandler::getResourceFile(std::string fileName, ResourceHandlerType type, bool addToOgre) {

	std::filesystem::path filePath;

	try
	{
		filePath = this->findAllInLocation(fileName, type);
	}
	catch (const std::exception&)
	{
		std::cout << "Resource File Not Found !";
		return NULL;
	}
	return filePath.string();

}


void ResourceHandler::getAllResources()
{

	//std::cout << "Loading All Resources" << std::endl;

	//// open Resource default location file
	//this->in_stream.open(SourceDir.string() + "/" + RESOURCE_DEFAULT_LOC);
	//
	//std::filesystem::current_path(SourceDir);


	//std::cout << "Current Working Path : " << std::filesystem::current_path() << std::endl;
	//
	//std::string line;
	//ResourceHandlerType resourceType = ResourceHandlerType::GLOBAL;
	//if (in_stream.is_open()) {
	//	while (std::getline(in_stream, line)) {
	//		try {
	//			//std::cout << "Finding At : " << line << std::endl;
	//			this->findAll(line,resourceType);
	//		}
	//		catch (const std::exception&e) {
	//			//std::cout << "Resource error " << e.what() << std::endl;
	//			resourceType = this->_getResourceLocationGroup(line);
	//		}

	//	}
	//	in_stream.close();
	//}
	//else {
	//	std::cout << std::endl <<  "Error Finding Resource Loc File";
	//}

}

//void ResourceHandler::addOgreRenderMeshResourceLocation()
//{
//	this->in_stream.open(RESOURCE_DEFAULT_LOC);
//	std::string line;
//
//	bool inRenderMeshes = false;
//
//	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(RENDER_MESH_LOC)) {
//		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(RENDER_MESH_LOC);
//	}
//
//	while (std::getline(in_stream,line))
//	{
//		if (inRenderMeshes && !line.empty()) {
//
//
//			std::string loc = line.substr(0, line.find_last_of('/'));
//
//			if (!std::filesystem::exists(line)) {
//				break;
//			}
//
//			if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(RENDER_MESH_LOC, loc.c_str())) {
//				std::cout << "path added " << loc << std::endl;
//				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem", RENDER_MESH_LOC);
//			}
//			
//		}
//		// In Resource line
//		if (line == RENDER_MESH_LOC) {
//			inRenderMeshes = true;
//		}
//	}
//
//	std::cout << "added : " << Ogre::ResourceGroupManager::getSingleton().listResourceNames(RENDER_MESH_LOC).get()->size() << std::endl;
//	this->getOgreRenderMeshes();
//
//}





ResourceHandler* ResourceHandler::GetInstance() {

	

	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new ResourceHandler();
		pinstance_->ini.SetUnicode();

	}
	return pinstance_;


}

void ResourceHandler::getCases(std::vector<Ogre::String>* outputVec)
{
	std::cout << "getcases run" << std::endl;
	if (!std::filesystem::exists(SourceDir.string() + "/Scenes"))
	{
		std::filesystem::create_directory(SourceDir.string() + "/Scenes");
	}

	for (const auto &entry : std::filesystem::directory_iterator(SourceDir.string() + "/Scenes"))
	{
		if (std::filesystem::is_directory(entry.status())) {
			outputVec->push_back(entry.path().filename().string());
			std::cout << "Case file : " << entry.path().filename().string() << std::endl;
		}
	}

}



// single value
void ResourceHandler::writeToFile(std::string key, std::string value , std::string section, std::string filename)
{
	//_LoadIniFile(_getSaveFileLoc(filename).string());

	/*ini.SetValue(section.c_str(), key.c_str(), value.c_str());

	ini.SaveFile(_getSaveFileLoc(filename).string().c_str());*/



}

void ResourceHandler::writeToFile(std::vector<SaveData>* data, std::string filename)
{
	/*_LoadIniFile(_getSaveFileLoc(filename).string());

	for (int i = 0; i < data->size(); i++)
	{
		ini.SetValue(data->at(i).section.c_str(), data->at(i).key.c_str(), data->at(i).value.c_str());
	}
	ini.SaveFile(_getSaveFileLoc(filename).string().c_str());
	ini.Reset();*/
}

void ResourceHandler::clearFile(std::string filename)
{
	/*std::ofstream outStream(_getSaveFileLoc(filename));
	if (out_stream.is_open()) {
		out_stream.clear();
		out_stream.close();
	}*/
	
}

bool ResourceHandler::fileExists(std::string filename)
{
	//return std::filesystem::exists(_getSaveFileLoc(filename));
}



std::string ResourceHandler::readFromFile(std::string key, std::string section, std::string filename)
{
	/*_LoadIniFile(_getSaveFileLoc(filename).string());

	const char* value = ini.GetValue(section.c_str(), key.c_str());

	if (value) {
		return value;
	}
	
	return "";*/

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

bool ResourceHandler::materialSaved(Ogre::String objectName, Ogre::String Material)
{
	
	//return std::filesystem::exists(_getSaveFileLoc(objectName));
	
}

//void ResourceHandler::saveScene(std::string scnName, std::string caseName, std::string Filename, int scnType)
//{
//
//	std::string masterLoc;
//	
//	if (scnType == 0) {
//		masterLoc = DYNAMIC_NODES_LOC;
//	}
//	else if (scnType == 1)
//	{
//		masterLoc = STATIC_NODES_LOC;
//	}
//	else if (scnType == 2)
//	{
//		masterLoc = MESH_NODES_LOC;
//	}
//	else {
//		throw ResourceHandlerFileNotFound();
//	}
//
//	if (!std::filesystem::exists(SourceDir.string() + "/Scenes"))
//	{
//		std::filesystem::create_directory(SourceDir.string() + "/Scenes");
//	}
//
//	if (!std::filesystem::exists(SourceDir.string() + "/Scenes/" + caseName))
//	{
//		std::filesystem::create_directory(SourceDir.string() + "/Scenes/" + caseName);
//	}
//
//	if (!std::filesystem::exists(SourceDir.string() + masterLoc))
//	{
//		std::filesystem::create_directory(SourceDir.string() + masterLoc);
//	}
//
//	std::string loc = masterLoc + Filename;
//
//	_LoadIniFile(loc);
//	
//	ini.SetValue(SECTION_SCENE, scnName.c_str(), "");
//	
//
//	ini.SaveFile(loc.c_str());
//
//	ini.Reset();
//
//
//}

void ResourceHandler::saveSceneObject(std::string filename, std::string caseName, SceneObject obj , int scnType)
{


	// Check if nodes folder exists
	std::string masterLoc;

	if (scnType == 0) {
		masterLoc = DYNAMIC_NODES_LOC;
	}
	else if (scnType == 1)
	{
		masterLoc = STATIC_NODES_LOC;
	}
	else if (scnType == 2)
	{
		masterLoc = MESH_NODES_LOC;
	}
	else {
		throw ResourceHandlerFileNotFound();
	}

	if (!std::filesystem::exists(SourceDir.string() + "/Scenes"))
	{
		std::filesystem::create_directory(SourceDir.string() + "/Scenes");
	}

	if (!std::filesystem::exists(SourceDir.string() + "/Scenes/" + caseName))
	{
		std::filesystem::create_directory(SourceDir.string() + "/Scenes/" + caseName);
	}

	if (!std::filesystem::exists(SourceDir.string() + masterLoc))
	{
		std::filesystem::create_directory(SourceDir.string() + masterLoc);
	}

	std::string loc = masterLoc + filename;

	_LoadIniFile(loc);

	ini.SetValue(SECTION_OBJECT, NODEKEY_NAME, obj.name.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_RENDERMESH, obj.RenderMesh.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_COLLIDERMESH, obj.ColliderMesh.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_PHYSXTYPE, obj.PhysXType.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_MASS, obj.mass.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_MATERIAL, obj.material.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_CASTSHADOW, obj.castShadow.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_RECEIVESHADOW, obj.receiveShadow.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_ROTATION, obj.rotation.c_str());
	ini.SetValue(SECTION_OBJECT, NODEKEY_POSITION, obj.position.c_str());

	

	ini.SaveFile(loc.c_str());

	ini.Reset();

}

bool ResourceHandler::scnExists(std::string filename, int scnType)
{
	std::string loc = SourceDir.string();
	if (scnType == 0) {
		loc += DYNAMIC_NODES_LOC + filename;
	}
	else if (scnType == 1)
	{
		loc += STATIC_NODES_LOC + filename;
	}
	else {
		loc += MESH_NODES_LOC + filename;
	}
	
	ini.LoadFile(loc.c_str());
	bool exists = ini.SectionExists(SECTION_SCENE);
	ini.Reset();
	return exists;

}

bool ResourceHandler::objExists(std::string filename , int scnType)
{
	std::string loc = SourceDir.string();
	if (scnType == 0) {
		loc += DYNAMIC_NODES_LOC + filename;
	}
	else if (scnType == 1)
	{
		loc += STATIC_NODES_LOC + filename;
	}
	else {
		loc += MESH_NODES_LOC + filename;
	}

	ini.LoadFile(loc.c_str());
	bool exists = ini.SectionExists(SECTION_OBJECT);
	ini.Reset();
	return exists;
}

void ResourceHandler::clearPrevSave()
{
	std::filesystem::remove_all(SourceDir.string() + DYNAMIC_NODES_LOC);
	std::filesystem::remove_all(SourceDir.string() + STATIC_NODES_LOC);
	std::filesystem::remove_all(SourceDir.string() + MESH_NODES_LOC);
}

std::vector<std::string> ResourceHandler::loadScene(std::string filename, int scnType)
{
	std::vector<std::string> scenes = std::vector<std::string>();

	std::string loc = SourceDir.string();
	if (scnType == 0) {
		loc += DYNAMIC_NODES_LOC + filename;
	}
	else if (scnType == 1)
	{
		loc += STATIC_NODES_LOC + filename;
	}
	else {
		loc += MESH_NODES_LOC + filename;
	}


	if (std::filesystem::exists(loc))
	{
		

		CSimpleIniA::TNamesDepend keys;
		_LoadIniFile(loc);

		ini.GetAllKeys(SECTION_SCENE, keys);

		if (keys.size() > 0)
		{
			for (const auto& entry : keys)
			{
				scenes.push_back(entry.pItem);
			}
		}


		ini.Reset();
		return scenes;

	}

	std::cout << "File : " << filename << " does not exists" << std::endl;
	return scenes;


}

SceneObject ResourceHandler::loadObject(std::string filename, int scnType)
{

	SceneObject obj = SceneObject();

	std::string loc = SourceDir.string();
	if (scnType == 0) {
		loc += DYNAMIC_NODES_LOC + filename;
	}
	else if (scnType == 1)
	{
		loc += STATIC_NODES_LOC + filename;
	}
	else {
		loc += MESH_NODES_LOC + filename;
	}

	SI_Error rc = ini.LoadFile(loc.c_str());
	if (rc < 0)
	{
		std::cout << "File not found" << std::endl;
	}

	obj.name = ini.GetValue(SECTION_OBJECT, NODEKEY_NAME);
	obj.RenderMesh = ini.GetValue(SECTION_OBJECT, NODEKEY_RENDERMESH);
	obj.ColliderMesh = ini.GetValue(SECTION_OBJECT, NODEKEY_COLLIDERMESH);
	obj.PhysXType = ini.GetValue(SECTION_OBJECT, NODEKEY_PHYSXTYPE);
	obj.mass = ini.GetValue(SECTION_OBJECT, NODEKEY_MASS);
	obj.material = ini.GetValue(SECTION_OBJECT, NODEKEY_MATERIAL);
	obj.castShadow = ini.GetValue(SECTION_OBJECT, NODEKEY_CASTSHADOW);
	obj.receiveShadow = ini.GetValue(SECTION_OBJECT, NODEKEY_RECEIVESHADOW);
	obj.rotation = ini.GetValue(SECTION_OBJECT, NODEKEY_ROTATION);
	obj.position = ini.GetValue(SECTION_OBJECT, NODEKEY_POSITION);

	//ini.Reset();

	return obj;
}



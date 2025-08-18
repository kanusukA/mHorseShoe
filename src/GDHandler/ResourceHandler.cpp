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

void ResourceHandler::read()
{

}

void ResourceHandler::write()
{
}

void ResourceHandler::addResource(std::filesystem::path filePath, ResourceHandlerType type) {

	switch (type)
	{
	case GLOBAL:
		break;
	case RENDER_MESH:
		if (!vectorContains(filePath.string(), renderMeshes)) {
			std::cout << "Added render meshes " << filePath.string() << std::endl;
			this->renderMeshes->push_back(filePath.string());
		}
		break;
	case COLLIDER_MESH:
		if (!vectorContains(filePath.string(), colliderMeshes)) {
			std::cout << "Added collider meshes " << filePath.string() << std::endl;
			this->colliderMeshes->push_back(filePath.string());
		}
		break;
	case MESH_MATERIALS:
		if (!vectorContains(filePath.string(), meshMaterials)) {
			std::cout << "Added mesh material " << filePath.string() << std::endl;
			this->meshMaterials->push_back(filePath.string());
		}
		break;
	case IMAGE:
		if (!vectorContains(filePath.string(), images)) {
			std::cout << "Added Image " << filePath.string() << std::endl;
			this->images->push_back(filePath.string());
		}
		break;
	default:
		break;
	}

}

std::filesystem::path ResourceHandler::_getSaveFileLoc(std::string filename)
{
	std::filesystem::path cur_path = std::filesystem::current_path();
	cur_path += "/save";
	cur_path += "/" + filename + ".txt";

	return cur_path;
}

void ResourceHandler::_readShaderFile(std::vector<std::string>* shaderVar, std::filesystem::path path)
{

	in_stream.open(path);
	std::string line;

	std::string word = "";

	bool uniFound = false;
	bool uniName = false;
	int type;

	std::cout << "Variables : " << std::endl;

	if (in_stream.is_open()) {
		while (std::getline(in_stream, line))
		{
			//std::cout << line << std::endl;

			// first check uniform keyword
			for (int i = 0; i < line.size(); i++)
			{
				if (line.at(i) != ' ' && line.at(i) != ',' && line.at(i) != ')') {
					word += line.at(i);
				}
				else {

					if (word == "uniform") {

						std::cout << "uniform" << std::endl;
						uniFound = true;

					}
					else if (uniFound) {
						std::cout << "type : ";

						uniFound = false;
						uniName = true;

						if (word == "int") {
							shaderVar->push_back("0");
							std::cout << "int" << std::endl;
						}
						else if (word == "float")
						{
							shaderVar->push_back("1");
							std::cout << "float" << std::endl;
						}
						else if (word == "float2")
						{
							shaderVar->push_back("2");
							std::cout << "float2" << std::endl;
						}
						else if (word == "float3")
						{
							shaderVar->push_back("3");
							std::cout << "float3" << std::endl;
						}
						else if (word == "float4")
						{
							shaderVar->push_back("4");
							std::cout << "float4" << std::endl;
						}
						else {
							std::cout << "invalid type" << std::endl;
							uniName = false;
						}


					}
					else if (uniName)
					{
						shaderVar->push_back(word);
						std::cout << "Word: " << word << std::endl;
						uniName = false;
					}
					word = "";
				}
			}

		}
	}

	in_stream.close();


}

void ResourceHandler::_LoadIniFile(std::string filename)
{
	SI_Error rc = ini.LoadFile(filename.c_str());
	if (rc < 0)
	{
		ini.SaveFile(filename.c_str());
		ini.LoadFile(filename.c_str());
	}
}

void ResourceHandler::findAll(std::string location, ResourceHandlerType type = ResourceHandlerType::GLOBAL) {

	if (location.empty()) {
		throw ResourceHandlerInvalidRequest();
	}

	try {
		for (const auto& entry : fs::directory_iterator(location)) {
			std::cout << "Adding resource : " << entry.path().string() << std::endl;
			this->addResource(entry.path().string(),type);
		}
	}
	catch (const std::exception& e) {
		throw ResourceHandlerFileNotFound();
	}
}

std::filesystem::path ResourceHandler::find(std::string fileName, std::string location)
{
	std::cout << "Opening file " << fileName << std::endl;

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
	in_stream.open(RESOURCE_DEFAULT_LOC);
	std::string line;

	if (in_stream.is_open()) {

		// Searching in Default Resource Loc
		while (std::getline(in_stream, line)) {
			try {
				std::filesystem::path path = this->find(filename, line);
				in_stream.close();
				return path;
			}
			catch (const std::exception&) {
				in_stream.close();
				throw ResourceHandlerFileNotFound();
			}
			
		}
		in_stream.close();
		throw ResourceHandlerFileNotFound();
		
	}

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

	std::cout << "Loading All Resources" << std::endl;

	// open Resource default location file
	this->in_stream.open(SourceDir.string() + "/" + RESOURCE_DEFAULT_LOC);
	
	std::filesystem::current_path(SourceDir);


	std::cout << "Current Working Path : " << std::filesystem::current_path() << std::endl;
	
	std::string line;
	ResourceHandlerType resourceType = ResourceHandlerType::GLOBAL;
	if (in_stream.is_open()) {
		while (std::getline(in_stream, line)) {
			try {
				std::cout << "Finding At : " << line << std::endl;
				this->findAll(line,resourceType);
			}
			catch (const std::exception&e) {
				std::cout << "Resource error " << e.what() << std::endl;
				resourceType = this->_getResourceLocationGroup(line);
			}

		}
		in_stream.close();
	}
	else {
		std::cout << std::endl <<  "Error Finding Resource Loc File";
	}

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

void ResourceHandler::addOgreRenderMeshResourceLocation()
{

	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(RENDER_MESH_LOC)) {
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(RENDER_MESH_LOC);
	}
	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(COLLIDER_MESH_LOC)) {
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(COLLIDER_MESH_LOC);
	}
	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(MESH_MATERIAL_LOC)) {
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(MESH_MATERIAL_LOC);
	}
	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(IMAGES_LOC)) {
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(IMAGES_LOC);
	}
	// Adds Meshes to Ogre
	for (int i = 0; i < renderMeshes->size(); i++)
	{

		std::string loc = renderMeshes->at(i).parent_path().string();

		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(loc,RENDER_MESH_LOC)) {
			std::cout << "path added : " << loc << std::endl;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem", RENDER_MESH_LOC);
		}

	} 

	// Adds Collider Meshes to Ogre , for conversion into physx
	for (int i = 0; i < colliderMeshes->size(); i++)
	{
		std::string loc = colliderMeshes->at(i).parent_path().string();

		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(loc, COLLIDER_MESH_LOC)) {
			std::cout << "path added : " << loc << std::endl;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem", COLLIDER_MESH_LOC);
		}
	}

	// Adds Materials to Ogre
	for (int i = 0; i < meshMaterials->size(); i++)
	{
		std::string loc = meshMaterials->at(i).parent_path().string();

		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(loc, MESH_MATERIAL_LOC)) {
			std::cout << "path added : " << loc << std::endl;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem", MESH_MATERIAL_LOC);
		}
	}

	// Adds Images to Ogre
	for (int i = 0; i < images->size(); i++)
	{
		std::string loc = images->at(i).parent_path().string();

		if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(loc, IMAGES_LOC)) {
			std::cout << "path added : " << loc << std::endl;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem", IMAGES_LOC);
		}
	}

	std::cout << "added meshes : " << Ogre::ResourceGroupManager::getSingleton().listResourceNames(RENDER_MESH_LOC).get()->size() << std::endl;
	std::cout << "added materials : " << Ogre::ResourceGroupManager::getSingleton().listResourceNames(MESH_MATERIAL_LOC).get()->size() << std::endl;
	std::cout << "added images : " << Ogre::ResourceGroupManager::getSingleton().listResourceNames(IMAGES_LOC).get()->size() << std::endl;
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	this->getOgreRenderMeshes();

}

void ResourceHandler::addOgreResourceLocation(std::filesystem::path location, std::string group = GENERAL_RESOURCE_LOC)
{
	try {
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(location.filename().string(), location.string(), group);
	}
	catch (const std::exception&) {
		std::cout << "unable to add Resource to Ogre";
	}

}

void ResourceHandler::getOgreRenderMeshes()
{
	try
	{

		Ogre::StringVectorPtr render_meshes = Ogre::ResourceGroupManager::getSingleton().listResourceNames(RENDER_MESH_LOC);
		
		for (int i = 0; i < render_meshes.get()->size(); i++)
		{
			if (render_meshes.get()->at(i).substr(render_meshes.get()->at(i).find(".")) != ".mesh") {
				render_meshes.get()->erase(render_meshes.get()->begin() + i);
			}
		}
		
		this->ogreRenderMeshes = render_meshes;
		// Add Material through resource handler to each render mesh!!
	}
	catch (const std::exception&)
	{
		
	}
	
}

void ResourceHandler::updateOgreMaterials()
{
	if (!OgreMaterials) {
		OgreMaterials = new std::vector<std::string>();
	}

	Ogre::ResourceManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();

	while (it.hasMoreElements()) {
		Ogre::ResourcePtr res = it.getNext();

		OgreMaterials->push_back(res.get()->getName());
	}

}


bool vectorContains(std::string term, std::vector<std::string>* vec) {

	if (vec) {

		for (int i = 0; i < vec->size(); i++)
		{
			std::cout << term.c_str() << "  :  " << vec->at(i).c_str() << std::endl;
			if (!strcmp(term.c_str(), vec->at(i).c_str())) {
				std::cout << "Already Exists" << std::endl;
				return true;
			}
		}
		return false;
	}
	else {
		return false;
	}
}

bool vectorContains(std::string term, std::vector<std::filesystem::path>* vec) {

	if (vec->empty()) {
		return false;
	}
	else {
		for (int i = 0; i < vec->size(); i++)
		{
			if (!strcmp(term.c_str(),vec->at(i).string().c_str())) {
				std::cout << "Already Exists" << std::endl;
				return true;
			}
		}
		return false;
	}
}



void ResourceHandler::readFile(std::string filename)
{

	// NOT YET USED
	
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

void ResourceHandler::readShaderFiles(Ogre::MaterialPtr mat)
{
	Ogre::String fragFileName = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getSourceFile();
	Ogre::String vertFileName = mat.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getSourceFile();
	Ogre::GpuProgramParametersPtr fragParam = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	Ogre::GpuProgramParametersPtr vertParam = mat.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters();

	if (!fragShaderVariables) {
		fragShaderVariables = new std::vector<std::string>();
	}
	else {
		fragShaderVariables->clear();
	}
	if (!vertShaderVariables) {
		vertShaderVariables = new std::vector<std::string>();
	}
	else {
		vertShaderVariables->clear();
	}

	std::filesystem::path vertfilepath;
	std::filesystem::path fragfilepath;

	for (int i = 0; i < meshMaterials->size(); i++)
	{
		if (meshMaterials->at(i).filename().string() == vertFileName)
		{
			vertfilepath = meshMaterials->at(i);
		}
		if (meshMaterials->at(i).filename().string() == fragFileName)
		{
			fragfilepath = meshMaterials->at(i);
		}
	}

	std::cout << "vert File loc : " << vertfilepath.relative_path().string() << std::endl;
	std::cout << "frag File loc : " << fragfilepath.relative_path().string() << std::endl;
	_readShaderFile(fragShaderVariables, fragfilepath);
	_readShaderFile(vertShaderVariables, vertfilepath);

	


}

void ResourceHandler::writeToFile(std::string key, std::string value, std::string filename)
{
	std::filesystem::path cur_path = std::filesystem::current_path();
	cur_path += "/save";

	if (!std::filesystem::exists(cur_path)) {
		std::filesystem::create_directories(cur_path);
	}

	cur_path += "/" + filename + ".txt";

	std::ofstream outStream(cur_path, std::ios::app | std::ios::out);

	outStream << key + "|" + value << "\n";
	
	

	outStream.close();




}

void ResourceHandler::clearFile(std::string filename)
{
	std::ofstream outStream(_getSaveFileLoc(filename));
	if (out_stream.is_open()) {
		out_stream.clear();
		out_stream.close();
	}
	
}

bool ResourceHandler::fileExists(std::string filename)
{
	return std::filesystem::exists(_getSaveFileLoc(filename));
}



std::string ResourceHandler::readFromFile(std::string key, std::string filename)
{

	std::ifstream inStream(_getSaveFileLoc(filename));
	std::string line;

	std::string word = "";

	if (inStream.is_open()) {
		while (std::getline(inStream,line))
		{
			word = "";
			for (int i = 0; i < line.size(); i++)
			{

				if (line.at(i) != '|') {
					word += line.at(i);
				}
				else {
					if (word == key) {
						inStream.close();
						return line.substr(i + 1, line.size());
					}
					else {
						word = "";
						continue;
					}
					
					
				}

			}
		}
	}
	inStream.close();
	return "";


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

void ResourceHandler::saveScene(std::string scnName, std::string Filename, int scnType)
{

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

	if (!std::filesystem::exists(SourceDir.string() + masterLoc))
	{
		std::filesystem::create_directory(SourceDir.string() + masterLoc);
	}

	std::string loc = masterLoc + Filename;

	_LoadIniFile(loc);

	ini.SetValue(SECTION_SCENE, scnName.c_str(), "");

	ini.SaveFile(loc.c_str());

	ini.Reset();


}

void ResourceHandler::saveSceneObject(std::string filename, SceneObject obj , int scnType)
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

	if (!std::filesystem::exists(SourceDir.string() + masterLoc))
	{
		std::filesystem::create_directory(SourceDir.string() + masterLoc);
	}

	std::string loc = masterLoc + filename;

	_LoadIniFile(loc);

	ini.SetValue(obj.name.c_str(), NODEKEY_RENDERMESH, obj.RenderMesh.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_COLLIDERMESH, obj.ColliderMesh.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_PHYSXTYPE, obj.PhysXType.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_MASS, obj.mass.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_MATERIAL, obj.material.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_CASTSHADOW, obj.castShadow.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_RECEIVESHADOW, obj.receiveShadow.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_ROTATION, obj.rotation.c_str());
	ini.SetValue(obj.name.c_str(), NODEKEY_POSITION, obj.position.c_str());

	ini.SaveFile(loc.c_str());

	ini.Reset();

}



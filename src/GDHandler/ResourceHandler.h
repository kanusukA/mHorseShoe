#pragma once

#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

//Local
#include<ResourceHandler/ResourceSaver.h>
#include<ResourceHandler/ResourceLoader.h>


// STL Headers
#include <list>


namespace fs = std::filesystem;


struct SaveData {
	std::string key;
	std::string value;
	std::string section;
};

struct SceneObject {
	std::string name;
	std::string RenderMesh = "";
	std::string ColliderMesh = "";
	std::string PhysXType = "";
	std::string mass = "";
	std::string position = "";
	std::string rotation = "";
	std::string castShadow = "";
	std::string receiveShadow = "";
	std::string material = "";
};



// Types of resources
enum ResourceHandlerType
{
	GLOBAL,
	RENDER_MESH,
	COLLIDER_MESH,
	MESH_MATERIALS,
	IMAGE
};





// Integrate it into gdhandler with Ogre 

class ResourceHandler : public ResourceHandlerBuilderContext, public ResourceSaver , public ResourceLoader, public ResourceReader
{

private:
	// Multi-Thread Shit
	static ResourceHandler* pinstance_;
	static std::mutex mutex_;

	ResourceHandlerType _getResourceLocationGroup(std::string groupStr);

	CSimpleIniA ini;

	// RESOURCES STORE
	// HERE GENERAL RESOURCES ARE STORED.

	// SOON TO BE DEPRICATED
	std::vector<std::filesystem::path>* MaterialDp = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* ShaderDp = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* TextureDp = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* MeshDp = new std::vector<std::filesystem::path>();

	// MASTER RESOURCE VECTOR
	// THIS VECTOR STORES ALL THE RESOURCE PATHS. THAT HAS BEEN INITALIZED! i.e. THE PATHS THAT HASS BEEN CONNECTED TO THE ENUM GROUP AND THE RESOURCE_LOAD_PATHS.
	std::vector<ResourceMasterGroup*>* masterResourceVector = new std::vector<ResourceMasterGroup*>();

	std::vector<std::filesystem::path>* fetchResourcesByEnum(ResourceLoaderEnums::ResourceLoadPaths group_p);

	// Searching Methods
	// Main search function
	void findAll(std::string location, ResourceHandlerType type);
	std::filesystem::path find(std::string filepath, std::string location);
	std::filesystem::path findAllInLocation(std::string filename, ResourceHandlerType type);

	
	// Add Resource
	//void addResource(std::filesystem::path filePath, ResourceHandlerType type);

	//std::filesystem::path _getSaveFileLoc(std::string filename);
	void _readShaderFile(std::vector<std::string>* shaderVar, std::filesystem::path path);

	// Loads ini file in ini obj. Creates the file if it does not exists
	void _LoadIniFile(std::string filename);

	// OVERHAUL PROJECT

	std::vector<std::string>* allResourceParentPaths = new std::vector<std::string>(13);


	// Hide the constructor and destructor of the class
protected:
	ResourceHandler();
	~ResourceHandler() {
		
	};

public:

	// OVERHAUL FUNCTIONS 
	// This function creates a Material and sets a unique_ptr. The ownership of incoming mat_p is taken.
	// Make sure Material is not a duplicate of existing
	 
	 
	// Checks if the folder structure and required files exists for Resource Tasks
	void checkFileStructure();

	// DEPRICATED - RECOURCES ARE NOT LOADED USING THE MASTER_RECOURCE_VECTOR. WHICH PROVIDES VEC POINTERS TO THE DP VECTORS.
	// Loads all the resources - Mesh, texture, Material, shader. Using the paths that are set on ResourceLoader.
	void loadResources();

	RLFetchedResource* fetchResourcesFromMesh(ResID meshID);


	void saveResources();


	std::vector<std::filesystem::path>* getMaterialsLoaded() { return MaterialDp; }
	std::vector<std::filesystem::path>* getShadersLoaded() { return ShaderDp; }
	std::vector<std::filesystem::path>* getTexturesLoaded() { return TextureDp; }
	std::vector<std::filesystem::path>* getRenderMeshLoaded() { return MeshDp; }

	// REOURCE LOADER FUNCTIONS
	void saveLoadPaths();

	// Assigns paths from ResourceMasterGruops and load_paths
	// requires ResourceLoader initalized.
	void setMasterLoadPaths();
	// fetches the data from load_paths and re-sets the ResourceMasterGruops
	void syncMasterLoadPaths();

	// Used to fetch Resource Paths for a specific group. Use RESOURCE_MASTER_GROUP_INDEX to fecth predefined groups.
	std::vector<std::filesystem::path>* fetchResourceGroupVecByIndex(int masterIndex);
	std::vector<std::filesystem::path>* fetchResourceGroupVecByName(std::string groupName);


	//Searching fullpath
	std::filesystem::path fetchLocByFileName(std::string filename_p, ResourceLoaderEnums::ResourceLoadPaths group_p);

	//PATHS
	void setPath(std::string path, ResourcePaths pathOf) {
		if (std::filesystem::exists(path))
		{
			allResourceParentPaths->at(pathOf) = path;
		}
		else {
			throw ResourceHandlerIDError(("Invalid Path - " + path).c_str());
		}
		
	}
	void setPath(std::string path, int pathPos) {
		if (std::filesystem::exists(path))
		{
			allResourceParentPaths->at(pathPos) = path;
		}
		else {
			throw ResourceHandlerIDError(("Invalid Path - " + path).c_str());
		}
	}
	std::string* getPath(ResourcePaths pathOf) {
		return &allResourceParentPaths->at(pathOf);
	}
	std::vector<std::string>* getPaths() {
		return allResourceParentPaths;
	}

	// Resources

	std::filesystem::path SourceDir;


	// Class should not be cloneable
	ResourceHandler(ResourceHandler& copy) = delete;

	// Class should not be assignable
	void operator=(const ResourceHandler&) = delete;

	std::string getResourceFile(std::string fileName, ResourceHandlerType type, bool addToOgre);

	std::vector<ResourceMasterGroup*>* getMasterResourceVector() {
		return masterResourceVector;
	}

	// Used for testing only
	void getAllResources();

	

	// OLDER METHODS _________________________________________________________________________________________________________

	// Ogre Functions
	//void addOgreRenderMeshResourceLocation(); // Adds default render mesh location.
	//void addOgreResourceLocation(std::filesystem::path location, std::string group);
	//void getOgreRenderMeshes();
	//void updateOgreMaterials();


	// Only way to initialize the class
	static ResourceHandler* GetInstance();

	// SAVE/LOAD FUNCTIONS

	// Gets saved Cases Name
	void getCases(std::vector<Ogre::String> *outputVec);

	//void readShaderFiles(Ogre::MaterialPtr mat);
	//void readShaderFiles(Ogre::GpuProgramPtr program);

	void writeToFile(std::string key, std::string value,std::string section, std::string filename);
	void writeToFile(std::vector<SaveData>* data, std::string filename);
	void clearFile(std::string filename);
	bool fileExists(std::string filename);
	std::string readFromFile(std::string key, std::string section, std::string filename);

	std::filesystem::path getSourceDir();

	bool materialSaved(Ogre::String objectName, Ogre::String Material = "");


	//void saveScene(std::string scnName, std::string caseName, std::string Filename, int scnType);
	void saveSceneObject(std::string filename, std::string caseName, SceneObject obj, int scnType);
	bool scnExists(std::string filename, int scnType);
	bool objExists(std::string filename, int scnType);
	void clearPrevSave();
	

	std::vector<std::string> loadScene(std::string filename, int scnType);
	SceneObject loadObject(std::string filename, int scnType);

	// IMAGES / TEXTURE
	/*void loadImgToTex();*/

	void shutdown() {
		this->saveLoadPaths();
		this->resource_loader_shutdown();
	}
};






// Exceptions
class ResourceHandlerDefaultLocNotFound : public std::exception {

public:
	char* what() {
		std::cout << std::endl << "Default Resouce Location File Was Not Found.";
	}

};
class ResourceHandlerFileNotFound : public std::exception {

public:
	char* what() {
		std::cout << std::endl << "File does not exsist in context.";
	}

};


class ResourceHandlerInvalidRequest : public std::exception {

public:
	char* what() {
		std::cout << std::endl << "The Request location or filename is invalid or unformatted";
	}

};

#endif // !RESOURCEHANDLER_H





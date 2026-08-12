#pragma once

#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

//Local
#include<ResourceHandler/ResourceSaver.h>
#include<ResourceHandler/ResourceLoader.h>
#include <ResourceHandler/ResourceGltf.h>


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




// Integrate it into gdhandler with Ogre 
class ResourceHandler : public ResourceHandlerBuilderContext, public ResourceSaver , public ResourceLoader, public ResourceReader , public ResourceGltf
{

private:
	// Multi-Thread Shit
	static ResourceHandler* pinstance_;
	static std::mutex mutex_;

	CSimpleIniA ini;


	// MASTER RESOURCE VECTOR
	// THIS VECTOR STORES ALL THE RESOURCE PATHS. THAT HAS BEEN INITALIZED! i.e. THE PATHS THAT HASS BEEN CONNECTED TO THE ENUM GROUP AND THE RESOURCE_LOAD_PATHS.
	std::vector<ResourceMasterGroup*>* masterResourceVector = new std::vector<ResourceMasterGroup*>(); 

	// Searching Methods
	// Main search function
	std::filesystem::path find(std::string filepath, std::string location);


protected:
	ResourceHandler();
	~ResourceHandler() {
		
	};

public:

	static ResourceHandler* GetInstance();

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

	// Use RESOURCE_MASTER_GROUP_INDEX to fecth predefined groups.
	std::filesystem::path* fetchFileInGroup(std::string filename_p, std::string group_p);


	// Resources
	std::filesystem::path SourceDir;

	//HELPING FUNCTIONS
	bool fileExists(std::string path) { return std::filesystem::exists(path); }

	// Class should not be cloneable
	ResourceHandler(ResourceHandler& copy) = delete;

	// Class should not be assignable
	void operator=(const ResourceHandler&) = delete;

	std::vector<ResourceMasterGroup*>* getMasterResourceVector() {
		return masterResourceVector;
	}

	std::filesystem::path getSourceDir();



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





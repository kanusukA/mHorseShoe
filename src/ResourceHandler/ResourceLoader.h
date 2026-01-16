#pragma once

//Local Header
#include <ResourceHandler/ResourceObjects.h>

// Third Headers
#include "SimpleIni.h"

//STL Headers
#include <fstream>

struct RLCase {
	long long id;
	std::string name;
	std::vector<long long> Scenes;
};
struct RLScene {
	long long id;
	std::string name;
	int scnType;
	float* position;
	float* rotation;
	float* scale;
	std::vector<long long> Scenes;
	std::vector<long long> objects;
};

struct RLObject {
	long long id;
	std::string name;
	int ObjectPhysxType;
	int mass;
	long long renderMeshID;
	long long colliderMeshID;
};

struct RLMesh {
	std::string name;
	long long materialID;
};

struct RLMaterial {
	std::string name;
	long long vertShaderID;
	long long fragShaderID;
	std::vector<ShaderTexture> textures;
};


struct RLShader {
	std::string name;
	std::string fileName;
	ShaderType type;
	std::vector<ShaderVar> shaderVars;
};

struct RLFetchedResource {
	RLMesh* mesh;
	RLMaterial* material;
	RLShader* shader;
};


class ResourceLoader {

private:
	CSimpleIniA* ini;
	std::string loadLocationIniPath;

	void loadSavedPaths();

	

protected:

	void resource_loader_shutdown() {
		this->~ResourceLoader();
	}

	// These paths are used to search objects and contents when using fetchAll function
	// The save formate uses ResourcePaths scheme
	std::vector<std::string>* load_paths = new std::vector<std::string>(7);

	void saveLoadPaths();

	

	// LOADING FILES AND FOLDERS
	void fetchPathContents(std::string path, std::string extension, std::vector<std::filesystem::path>* output, bool searchFolders = false);



	RLFetchedResource* _fetchedResourcesFromMesh(ResID meshID, std::vector<std::string>* resourcePaths_p);

	RLMesh* _fetchMesh(ResID id, std::string path_p);
	RLMaterial* _fetchMaterial(ResID id, std::string path_p, std::string matTexPath_p);
	RLShader* _fetchShader(ResID id, std::string path_p) { return nullptr; };
	ShaderTexture _fetchShaderTexture(ResID id, std::string path_p) { return ShaderTexture(); };
	




public:

	std::vector<RLCase>* RLCases = new std::vector<RLCase>();
	std::vector<RLScene>* RLScenes = new std::vector<RLScene>();
	std::vector<RLObject>* RLObjects = new std::vector<RLObject>();


	~ResourceLoader() {
		saveLoadPaths();
		delete load_paths;
	}

	// USE ResourceLoadPaths Enum to fecth specific load paths
	std::vector<std::string>* getLoadPaths() {
		return load_paths;
	}

	void addLoadPath(ResourceLoaderEnums::ResourceLoadPaths pathType_p, std::string path_p);
	void addLoadPath(std::string path_p);

	// loadLocation_p - Resource Loader's path saving directory
	void initResourceLoader(CSimpleIniA* ini_p, std::string loadLocation_p) {
		ini = ini_p;
		loadLocationIniPath = loadLocation_p;

		loadSavedPaths();

	}

	// Loading Saved Data
	void loadSavedCases(std::string path_p);
	void loadSavedScenes(std::string path_p);
	void loadSavedObject(std::string path_p);

	



	// Loading Things Not related to Saved Data
	void loadMaterialsDp(std::vector<std::filesystem::path>* output, std::string extension = ".material", bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadShadersDp(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadMeshesDp(std::vector<std::filesystem::path>* output, std::string extension = ".mesh", bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadTexturesDp(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);



	

};


class ResourceHandlerLoaderError : public std::exception {
private:
	const char* error;
public:

	ResourceHandlerLoaderError(const char* error_p) {
		error = error_p;
	}

	char* what() {
		std::cout << error << std::endl;
	}

};
#pragma once

//Local Header
#include <ResourceHandler/ResourceObjects.h>

// Third Headers
#include "SimpleIni.h"

//STL Headers
#include <fstream>

struct RLShader {
	std::string name;
	std::string fileName;
	ShaderType type;
	std::vector<ShaderVar> shaderVars;
};

struct RLMaterial {
	std::string name;
	std::string materialFilePath;
	RLShader vertShader;
	RLShader fragShader;
	std::vector<ShaderTexture> textures;
	bool wireframe;
};

struct RLMesh {
	std::string name;
	std::string filepath;
};

struct RLObject {
	std::string name;
	int ObjectPhysxType;
	int mass;
	RLMesh mesh;
	RLMaterial material;
};

struct RLScene {
	std::string name;
	int scnType;
	float* position;
	float* rotation;
	float* scale;
	std::vector<RLObject> objects;
};

struct RLCase {
	long long id;
	std::string name;
	std::vector<RLScene> Scenes;
};


struct RLFetchedResource {
	RLMesh* mesh;
	RLMaterial* material;
	RLShader* shader;
};


class ResourceLoader {

private:

	CSimpleIniA* ini;
	std::filesystem::path resourceLoaderIniPath;
	std::filesystem::path dataDirPath;
	std::filesystem::path sourceDir;


	void loadSavedPaths();

	std::vector<std::filesystem::path> caseFileNames = std::vector<std::filesystem::path>();
	

protected:

	

	void resource_loader_shutdown() {
		this->~ResourceLoader();
	}

	// These paths are used to search objects and contents when using fetchAll function
	// The save formate uses ResourcePaths scheme
	std::vector<ResourceLoadPath>* load_paths = new std::vector<ResourceLoadPath>();


	// Main File through which details of files are fetched
	YAML::Node loadFile;

	
	RLShader* _fetchShader(ResID id, std::string path_p) { return nullptr; };
	ShaderTexture _fetchShaderTexture(ResID id, std::string path_p) { return ShaderTexture(); };
	

public:

	std::vector<RLCase>* RLCases = new std::vector<RLCase>();
	std::vector<RLScene>* RLScenes = new std::vector<RLScene>();
	std::vector<RLObject>* RLObjects = new std::vector<RLObject>();


	~ResourceLoader() {
		delete load_paths;
	}

	// LOAD PATHS
	// USE ResourceLoadPaths Enum to fetch specific load paths
	std::vector<ResourceLoadPath>* getLoadPaths() {
		return load_paths;
	}

	// Check ResourceMasterGroups for defined group names
	ResourceLoadPath* getLoadPath(std::string groupName);

	void addLoadPath(ResourceLoadPath path_p);

	void removeLoadPath(int index) {
		load_paths->erase(load_paths->begin() + index);
	}

	// loadLocation_p - Resource Loader's path saving directory
	void initResourceLoader(CSimpleIniA* ini_p,std::filesystem::path sourceDir_p,std::filesystem::path dataDirPath_p , std::string resourceLoaderIniPath_p) {
		ini = ini_p;
		sourceDir = sourceDir_p;
		resourceLoaderIniPath = resourceLoaderIniPath_p;
		dataDirPath = dataDirPath_p;
		/*loadSavedPaths();
		loadSavedCases();*/

	}

	void loadSavedCases();

	RLCase fetchCaseData(std::filesystem::path yamlFilePath);

	

	// LOADING FILES AND FOLDERS
	void fetchPathContents(std::string path, std::string extension, std::vector<std::filesystem::path>* output, bool searchFolders = false);

	// Loading Saved Data
	//void loadSavedCases(std::string path_p);

	// GETTER
	std::vector<std::filesystem::path>* getSavedCaseFiles() {
		return &caseFileNames;
	}

	// Open functions
	bool openLoadFile(std::string filepath);

	std::string fetchNodeByMapKey(std::string key);



	

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
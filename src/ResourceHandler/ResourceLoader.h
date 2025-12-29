#pragma once

//Local Header
#include <ResourceHandler/ResourceObjects.h>

// Third Headers
#include "SimpleIni.h"

//STL Headers
#include <fstream>

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

public:


	~ResourceLoader() {
		saveLoadPaths();
		delete load_paths;
	}

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


	// Loading Things
	void loadMaterials(std::vector<std::filesystem::path>* output, std::string extension = ".material", bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadShaders(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadRenderMesh(std::vector<std::filesystem::path>* output, std::string extension = ".mesh", bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadColliderMesh(std::vector<std::filesystem::path>* output, std::string extension = ".mesh", bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);
	void loadTextures(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources = false, bool searchFolders = false, bool clearOutput = false);




	

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
#pragma once

#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H
#include<Windows.h>
#include <Ogre.h>

#include <mutex>
#include <fstream>
#include <iostream>
#include <string>

#include <filesystem>
#include <list>

#include "SimpleIni.h"



namespace fs = std::filesystem;


#define RESOURCE_DEFAULT_LOC "resource_handler_def_loc.txt"
#define GENERAL_RESOURCE_LOC "General_Resource_Loc"
#define RENDER_MESH_LOC "Render_Mesh"
#define COLLIDER_MESH_LOC "Collider_Mesh"
#define MESH_MATERIAL_LOC "Mesh_Materials"
#define IMAGES_LOC "Images"

#define DYNAMIC_NODES_LOC "./Scenes/Dynamic/"
#define DYNAMIC_NODES_FILE "dynamicNodes.ini"

#define STATIC_NODES_LOC "./Scenes/Static/"
#define STATIC_NODES_FILE "StaticNodes.ini"

#define MESH_NODES_LOC "./Scenes/Mesh/"
#define MESH_NODES_FILE "MeshNodes.ini"

#define SECTION_SCENE "SCENE"

#define NODEKEY_RENDERMESH "RENDER_MESH"
#define NODEKEY_COLLIDERMESH "COLLIDER_MESH"
#define NODEKEY_PHYSXTYPE "PHYSXTYPE"
#define NODEKEY_MASS "MASS"
#define NODEKEY_POSITION "POSITION"
#define NODEKEY_ROTATION "ROTATION"
#define NODEKEY_CASTSHADOW "CAST_SHADOW"
#define NODEKEY_RECEIVESHADOW "RECEIVE_SHADOW"
#define NODEKEY_MATERIAL "MATERIAL"

struct SceneObject {
	std::string name = "";
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

class ResourceHandler
{

private:
	// Multi-Thread Shit
	static ResourceHandler* pinstance_;
	static std::mutex mutex_;

	std::ofstream out_stream;
	std::ifstream in_stream;
	std::fstream f_stream;

	ResourceHandlerType _getResourceLocationGroup(std::string groupStr);

	CSimpleIniA ini;


	// default locations


	void read();
	void write();

	// Searching Methods
	// Main search function
	void findAll(std::string location, ResourceHandlerType type);
	std::filesystem::path find(std::string filepath, std::string location);
	std::filesystem::path findAllInLocation(std::string filename, ResourceHandlerType type);

	

	// Add Resource
	void addResource(std::filesystem::path filePath, ResourceHandlerType type);

	std::filesystem::path _getSaveFileLoc(std::string filename);
	void _readShaderFile(std::vector<std::string>* shaderVar, std::filesystem::path path);

	// Loads ini file in ini obj. Creates the file if it does not exists
	void _LoadIniFile(std::string filename);

	// Hide the constructor and destructor of the class
protected:
	ResourceHandler() {
		this->SourceDir = getSourceDir();
	};
	~ResourceHandler() {};


public:

	// Resources
	// RenderMeshes
	// OPTIMIZE THIS !!!!!!
	//  Can be changed to an Observable state in future for regulation of use
	std::vector<std::filesystem::path>* renderMeshes = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* meshMaterials = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* colliderMeshes = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* images = new std::vector<std::filesystem::path>();
	std::vector<std::filesystem::path>* globalResources = new std::vector<std::filesystem::path>();
	std::vector<std::string>* OgreMaterials;
	std::vector<std::string>* fragShaderVariables;
	std::vector<std::string>* vertShaderVariables;

	std::filesystem::path SourceDir;

	Ogre::StringVectorPtr ogreRenderMeshes = Ogre::StringVectorPtr();

	// Class should not be clonable
	ResourceHandler(ResourceHandler& copy) = delete;

	// Class should not be assignable
	void operator=(const ResourceHandler&) = delete;

	void readFile(std::string filename);

	std::string getResourceFile(std::string fileName, ResourceHandlerType type, bool addToOgre);

	// Used for testing only
	void getAllResources();


	// Ogre Functions
	void addOgreRenderMeshResourceLocation(); // Adds default render mesh location.
	void addOgreResourceLocation(std::filesystem::path location, std::string group);
	void getOgreRenderMeshes();
	void updateOgreMaterials();


	// Only way to initalize the class
	static ResourceHandler* GetInstance();

	void readShaderFiles(Ogre::MaterialPtr mat);

	void writeToFile(std::string key, std::string value, std::string filename);
	void clearFile(std::string filename);
	bool fileExists(std::string filename);
	std::string readFromFile(std::string key, std::string filename);

	std::filesystem::path getSourceDir();

	void saveScene(std::string scnName, std::string Filename, int scnType);
	void saveSceneObject(std::string filename, SceneObject obj, int scnType);

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





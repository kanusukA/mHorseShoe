#pragma once

#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

//Local
#include<ResourceHandler/ResourceSaver.h>
#include <GDHandler/ResourcePaths.h>
#include <Gui/GuiComponents/ToastComponent.h>

// STL Headers
#include<Windows.h>
#include <list>


namespace fs = std::filesystem;


struct SaveData {
	std::string key;
	std::string value;
	std::string section;
};

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

class ResourceHandler : public ResourceHandlerBuilderContext
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


	// RESOURCES BUILDER
	void createCase(CaseResource* case_p) override {
		case_p->setId(this->getCaseIndex());
		AddIndexToMaster(case_p->getId());
		this->addCaseRes(case_p);
	}

	void createScene(SceneResource* scn_p) override {
		scn_p->setId(this->getSceneIndex());
		AddIndexToMaster(scn_p->getId());
		this->addSceneRes(scn_p);
	}

	void createObject(ObjectResource* obj_p) override {
		obj_p->setId(this->getObjectIndex());
		AddIndexToMaster(obj_p->getId());
		this->addObjectRes(obj_p);
	}

	void createMaterial(MaterialResource* mat_p) override {
		mat_p->setId(this->getMaterialIndex());
		AddIndexToMaster(mat_p->getId());
		this->addMaterialRes(mat_p);
	}

	void createShader(ShaderResource* shader_p) override {
		shader_p->setId(this->getShaderIndex());
		AddIndexToMaster(shader_p->getId());
		this->addShaderRes(shader_p);
	}

	void createRenderMesh(RenderMeshResource* render_p) override {
		render_p->setId(this->getRenderMeshIndex());
		AddIndexToMaster(render_p->getId());
		this->addRenderMeshRes(render_p);
	}

	void createColliderMesh(ColliderMeshResource* col_p) override {
		col_p->setId(this->getColliderMeshIndex());
		AddIndexToMaster(col_p->getId());
		this->addColliderMeshRes(col_p);
	}

	void createImage(ImageResource* image_p) override {
		image_p->setId(this->getImageIndex());
		AddIndexToMaster(image_p->getId());
		this->addImageRes(image_p);
	}

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

	// OVERHAUL PROJECT
	void createFolder(std::filesystem::path path);
	void createFile(std::filesystem::path path);

	
	std::vector<std::string>* paths = new std::vector<std::string>(13);


	// Hide the constructor and destructor of the class
protected:
	ResourceHandler();
	~ResourceHandler() {};


public:

	// OVERHAUL FUNCTIONS
	// Checks if the folder structure and required files exists for Resource Tasks
	void checkFileStructure();

	//PATHS
	void setPath(std::string path, ResourcePaths pathOf) {
		if (std::filesystem::exists(path))
		{
			paths->at(pathOf) = path;
		}
		else {
			throw ResourceHandlerIDError(("Invalid Path - " + path).c_str());
		}
		
	}
	void setPath(std::string path, int pathPos) {
		if (std::filesystem::exists(path))
		{
			paths->at(pathPos) = path;
		}
		else {
			throw ResourceHandlerIDError(("Invalid Path - " + path).c_str());
		}
	}
	std::string* getPath(ResourcePaths pathOf) {
		return &paths->at(pathOf);
	}
	std::vector<std::string>* getPaths() {
		return paths;
	}

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

	// SAVE/LOAD FUNCTIONS

	// Gets saved Cases Name
	void getCases(std::vector<Ogre::String> *outputVec);

	void readShaderFiles(Ogre::MaterialPtr mat);

	void writeToFile(std::string key, std::string value,std::string section, std::string filename);
	void writeToFile(std::vector<SaveData>* data, std::string filename);
	void clearFile(std::string filename);
	bool fileExists(std::string filename);
	std::string readFromFile(std::string key, std::string section, std::string filename);

	std::filesystem::path getSourceDir();

	bool materialSaved(Ogre::String objectName, Ogre::String Material = "");


	void saveScene(std::string scnName, std::string caseName, std::string Filename, int scnType);
	void saveSceneObject(std::string filename, std::string caseName, SceneObject obj, int scnType);
	bool scnExists(std::string filename, int scnType);
	bool objExists(std::string filename, int scnType);
	void clearPrevSave();
	

	std::vector<std::string> loadScene(std::string filename, int scnType);
	SceneObject loadObject(std::string filename, int scnType);

	// IMAGES / TEXTURE
	void loadImgToTex();


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





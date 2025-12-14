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
#define SECTION_OBJECT "OBJECT"
#define SECTION_PARENT_SCN "PARENT_NODE"
#define KEY_PARENT_SCN "NODE"

#define SECTION_MATERIAL "Material"
#define SECTION_VERTEX_SHADER "Vertex Param"
#define SECTION_FRAGMNET_SHADER "Fragment Param"
#define SECTION_TEXTURE "Textures"

#define NODEKEY_NAME "NAME"
#define NODEKEY_RENDERMESH "RENDER_MESH"
#define NODEKEY_COLLIDERMESH "COLLIDER_MESH"
#define NODEKEY_PHYSXTYPE "PHYSXTYPE"
#define NODEKEY_MASS "MASS"
#define NODEKEY_POSITION "POSITION"
#define NODEKEY_ROTATION "ROTATION"
#define NODEKEY_CASTSHADOW "CAST_SHADOW"
#define NODEKEY_RECEIVESHADOW "RECEIVE_SHADOW"
#define NODEKEY_MATERIAL "MATERIAL"



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



typedef unsigned long long ResID;

class ResourceHandlerIDError : public std::exception {
private:
	const char* cause;
public:
	ResourceHandlerIDError(const char* cause_p) {
		cause = cause_p;
	};
	char* what() {
		std::cout << std::endl << cause << std::endl;
	}

};


class CaseResource;
class SceneResource;


// MASTER RESOURCE CLASS. HANDLES STORAGE OF RESOURCE IDS and RESOURCES
class ResourceHandlerBuilderContext {

private:
	std::vector<ResID>* masterList = new std::vector<ResID>();

protected:

	std::vector<CaseResource>* caseRes = new std::vector<CaseResource>();
	std::vector<SceneResource>* scnRes = new std::vector<SceneResource>();

	void AddIndexToMaster(ResID id) {

		for (int i = 0; i < masterList->size(); i++)
		{
			if (masterList->at(i) == id)
			{
				throw ResourceHandlerIDError("Duplicate ID begin Created!");
			}
		}
		masterList->push_back(id);

	}

	int getCaseIndex() {
		return caseRes->size();
	}
	int getSceneIndex() {
		return scnRes->size();
	}

	void addCaseRes(CaseResource* case_p) {
		caseRes->push_back(*case_p);
	}

	void addSceneRes(SceneResource* scn_p) {
		scnRes->push_back(*scn_p);
	}

public:

	std::vector<ResID>* getMasterList() { return masterList; };

	// CASE RESOURCE

	// Not a suggested method to fetch few cases. Try using getByID() insted. This method is only for GUI applications!
	std::vector<CaseResource>* getAllCase() { return caseRes; };
	std::vector<SceneResource>* getAllScenes() { return scnRes; };

	SceneResource* fetchSceneResourceByID(ResID id) {
		return &scnRes->at(id - 10100000000);
	}

	// Initalizes CaseResource with resource handler and sets id and name!
	virtual void createCase(CaseResource* case_p) {};

	virtual void createScene(SceneResource* scn_p) {};


};


class Resource {
protected:
	bool init = false;
	ResID _id;
	virtual void setId(int index) {};

	std::string name;

public:

	virtual void build(ResourceHandlerBuilderContext* context) {}

	void setName(std::string name_p) {
		name = name_p;
	}

	std::string getName() {
		return name;
	}


	ResID getId() { return _id; };

};



class CaseResource : public Resource {
private:

	std::vector<ResID>* Scenes = new std::vector<ResID>();


public:

	// RESOURCE SPECIFIC FUNCTIONS

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10000000000 + index;
	}


	CaseResource(const char* name_p) {
		this->setName(name_p);
	};

	CaseResource(ResourceHandlerBuilderContext* context, const char* name_p) {
		build(context, name_p);
	}

	void build(ResourceHandlerBuilderContext* context, const char* name_p) {
		this->setName(name_p);
		context->createCase(this);
	}

	// CASE METHODS

	std::vector<ResID>* getScenesInCase() {
		return Scenes;
	}

	void addSceneToCase(ResID sceneID) {
		if (sceneID >= 10100000000 && sceneID < 10200000000) {
			Scenes->push_back(sceneID);
		}
		else {
			throw ResourceHandlerIDError(("Invalid ID Entered. required : SceneResourceID , entered : " + std::to_string(sceneID)).c_str());
		}
	}

	void removeSceneByID(ResID sceneID) {
		for (int i = 0; i < Scenes->size(); i++)
		{
			if (Scenes->at(i) == sceneID)
			{
				Scenes->erase(Scenes->begin() + i);
				break;
			}
		}
	}

	void removeSceneByIndex(int index) {
		Scenes->erase(Scenes->begin() + index);
	}

};

class SceneResource : public Resource
{
protected:

	int scnType;

	Ogre::Vector3 position;
	Ogre::Vector4 orientation;
	Ogre::Vector3 scale;


	std::vector<ResID>* objects = new std::vector<ResID>();

public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10100000000 + index;
	}

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, int SceneType, Ogre::Vector3 position_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) {

		scnType = SceneType;
		position = position_p;
		orientation = orientation_p;
		scale = scale_p;

		this->setName(name_p);
		
		context->createScene(this);

	};

	void addObject(ResID objectID) {
		if (objectID >= 10200000000 && objectID < 10300000000)
		{
			objects->push_back(objectID);
		}
		else {
			throw ResourceHandlerIDError(("Invalid ID Entered. required : SceneResourceID , entered : " + std::to_string(objectID)).c_str());
		}
	}

	void removeObjectById(ResID objectID) {
		for (int i = 0; i < objects->size(); i++)
		{
			if (objects->at(i) == objectID)
			{
				objects->erase(objects->begin() + i);
				break;
			}
		}
	}

	void removeObjectByIndex(int index) {
		objects->erase(objects->begin() + index);
	}

	std::vector<ResID>* getObjects() {
		return objects;
	}

};

class ObjectResource : public Resource
{

private:

	const char* name;
	float mass;
	int physXType;

	ResID renderMeshID;
	ResID colliderMeshID;

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index;
	}


public:
	ObjectResource(const char* name_p, int index, int objectType, ResID material_p, ResID renderMesh_p, ResID colliderMesh_p, float mass_p) {
		setId(index);
		name = name_p;

		_id += 10000000 * objectType;

		physXType = objectType;
		renderMeshID = renderMesh_p;
		colliderMeshID = colliderMesh_p;
		mass = mass_p;

	}

};

enum ShaderVarType
{
	INTEGER,
	FLOAT0,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	RBOOL
};

struct ShaderVar {

	std::string varName;
	ShaderVarType varType;

	int* varInt = new int(0);
	float* varFloat = new float(0.0);
	float varFloat2[2] = { 0.0,0.0 };
	float varFloat3[3] = { 0.0, 0.0, 0.0 };
	float varFloat4[4] = { 0.0, 0.0, 0.0, 0.0 };

};

class MaterialResource : public Resource
{

private:
	const char* materialName;

	const char* VertexShaderName;
	const char* FragmentShaderName;

	ResID DiffuseTexture;
	ResID RoughnessTexture;
	ResID NormalTexture;
	ResID ParallaxTexture;

	std::vector<ResID>* Textures = new std::vector<ResID>();

	// These Parameters contain presaved values of Material and must be cross checked with Ogre Material's parameters for consistancy
	std::vector<ShaderVar>* VertexParameters = new std::vector<ShaderVar>();
	std::vector<ShaderVar>* FragmentParameters = new std::vector<ShaderVar>();

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10700000000 + index;
	}

public:

	MaterialResource(const char* name_p,
		int index,
		const char* vertex_name_p,
		const char* fragment_name_p)
	{
		setId(index);
		VertexShaderName = vertex_name_p;
		FragmentShaderName = fragment_name_p;
	}

	void addVertexShaderVar(ShaderVar vertexShaderVar) {
		VertexParameters->push_back(vertexShaderVar);
	}

	void addFragmentShaderVar(ShaderVar fragmentShaderVar) {
		FragmentParameters->push_back(fragmentShaderVar);
	}

	void addDiffuseTexture(ResID Diffuse_p) {
		DiffuseTexture = Diffuse_p;
	}
	void addRoughnessTexture(ResID Roughness_p) {
		RoughnessTexture = Roughness_p;
	}
	void addParallaxTexture(ResID Parallax_p) {
		ParallaxTexture = Parallax_p;
	}
	void addNormalTexture(ResID Normal_p) {
		NormalTexture = Normal_p;
	}

	void addTexture(ResID Texture) {
		Textures->push_back(Texture);
	}

	void removeTextureByID(ResID Texture) {
		for (int i = 0; i < Textures->size(); i++)
		{
			if (Textures->at(i) == Texture)
			{
				Textures->erase(Textures->begin() + i);
				break;
			}
		}
	}

	void removeTextureByIndex(int index) {
		Textures->erase(Textures->begin() + index);
	}

};



class ImageResource : public Resource
{
private:

	std::filesystem::path imagePath;

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10600000000 + index;

	}

public:

	ImageResource(int index, std::filesystem::path imagePath_p) {
		setId(index);
		imagePath = imagePath_p;
	}

};

class RenderMeshResource : public Resource
{
private:

	const char* name;

	ResID Material;

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}
public:

	RenderMeshResource(const char* name_p, int index) {
		setId(index);
		name = name_p;
	}

	virtual void setMaterial(ResID material_p) {}

};

class ColliderMeshResource : public Resource
{
private:

	const char* name;

	ResID Material;

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10400000000 + index;

	}
public:

	ColliderMeshResource(const char* name_p, int index) {
		setId(index);
		name = name_p;
	}

	virtual void setMaterial(ResID material_p) {}

};

class MeshResource : public Resource
{
private:

	const char* name;

	ResID Material;

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10500000000 + index;

	}
public:

	MeshResource(const char* name_p, int index) {
		setId(index);
		name = name_p;
	}

	virtual void setMaterial(ResID material_p) {}

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
		this->addCaseRes(case_p);
		this->AddIndexToMaster(case_p->getId());
	}

	void createScene(SceneResource* scn_p) override {
		scn_p->setId(this->getSceneIndex());
		this->addSceneRes(scn_p);
		this->AddIndexToMaster(scn_p->getId());
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

	// Hide the constructor and destructor of the class
protected:
	ResourceHandler() {
		this->SourceDir = getSourceDir();
		this->colliderMeshes->push_back("box");
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





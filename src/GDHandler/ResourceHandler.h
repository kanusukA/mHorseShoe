#pragma once

#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <Gui/GuiComponents/ToastComponent.h>
#include <GDHandler/ResourcePaths.h>

#include<Windows.h>
#include <Ogre.h>

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

enum PhysXType {
	Static,
	Dynamic,
	Kinematic
};

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
class ObjectResource;
class ShaderResource;
class MaterialResource;
class RenderMeshResource;
class ColliderMeshResource;
class ImageResource;



// MASTER RESOURCE CLASS. HANDLES STORAGE OF RESOURCE IDS and RESOURCES
class ResourceHandlerBuilderContext {

private:
	std::vector<ResID>* masterList = new std::vector<ResID>();

protected:

	std::vector<CaseResource*>* caseRes = new std::vector<CaseResource*>();
	std::vector<SceneResource*>* scnRes = new std::vector<SceneResource*>();
	std::vector<ObjectResource*>* objRes = new std::vector<ObjectResource*>();
	std::vector<ShaderResource*>* shaderRes = new std::vector<ShaderResource*>();
	std::vector<MaterialResource*>* matRes = new std::vector<MaterialResource*>();
	std::vector<RenderMeshResource*>* renderRes = new std::vector<RenderMeshResource*>();
	std::vector<ColliderMeshResource*>* colRes = new std::vector<ColliderMeshResource*>();
	std::vector<ImageResource*>* imageRes = new std::vector<ImageResource*>();

	// Items Must be added to the Master List before Their respective add Function!
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

	void validateMasterList() {
		unsigned long long totalItems = caseRes->size() + scnRes->size() + objRes->size() + 
			shaderRes->size() + matRes->size() + renderRes->size() + colRes->size() + imageRes->size();
		if (totalItems != masterList->size())
		{
			throw ResourceHandlerIDError("Master List is not valid!");
		}
	}

	int getCaseIndex() {
		return caseRes->size();
	}
	int getSceneIndex() {
		return scnRes->size();
	}
	int getObjectIndex() {
		return objRes->size();
	}
	int getMaterialIndex() {
		return matRes->size();
	}
	int getShaderIndex() {
		return shaderRes->size();
	}
	int getRenderMeshIndex() {
		return renderRes->size();
	}
	int getColliderMeshIndex() {
		return colRes->size();
	}
	int getImageIndex() {
		return imageRes->size();
	}

	void addCaseRes(CaseResource* case_p) {
		caseRes->push_back(case_p);
		validateMasterList();
	}

	void addSceneRes(SceneResource* scn_p) {
		
		scnRes->push_back(scn_p);
		validateMasterList();
	}

	void addObjectRes(ObjectResource* obj_p) {
		
		objRes->push_back(obj_p);
		validateMasterList();
	}

	void addShaderRes(ShaderResource* shader_p) {
		
		shaderRes->push_back(shader_p);
		validateMasterList();
	}

	void addMaterialRes(MaterialResource* mat_p) {
		
		matRes->push_back(mat_p);
		validateMasterList();
	}

	void addRenderMeshRes(RenderMeshResource* render_p) {
		renderRes->push_back(render_p);
		validateMasterList();
	}

	void addColliderMeshRes(ColliderMeshResource* collider_p) {
		colRes->push_back(collider_p);
		validateMasterList();
	}

	void addImageRes(ImageResource* image_p) {
		imageRes->push_back(image_p);
		validateMasterList();
	}

public:

	std::vector<ResID>* getMasterList() { return masterList; };

	// CASE RESOURCE

	// Not a suggested method to fetch few cases. Try using getByID() insted. This method is only for GUI applications!
	std::vector<CaseResource*>* getAllCase() { return caseRes; };
	std::vector<SceneResource*>* getAllScenes() { return scnRes; };
	std::vector<ObjectResource*>* getAllObjects() { return objRes; };
	std::vector<MaterialResource*>* getAllMaterial(){ return matRes; };
	std::vector<ShaderResource*>* getAllShader() { return shaderRes; };
	std::vector<RenderMeshResource*>* getAllRenderMesh() { return renderRes; };
	std::vector<ColliderMeshResource*>* getAllColliderMesh() { return colRes; };
	std::vector<ImageResource*>* getAllImages() { return imageRes; };

	SceneResource* fetchSceneResourceByID(ResID id) {
		try{
			if (id < 10110000000)
			{
				return scnRes->at(id - 10100000000);
			}
			else if (id < 10120000000)
			{
				return scnRes->at(id - 10110000000);
			}
			else {
				return scnRes->at(id - 10120000000);
			}
			
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
		
	}
	CaseResource* fetchCaseResourceByID(ResID id) {
		try
		{
			return caseRes->at(id - 10000000000);
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
		
	}
	ShaderResource* fetchShaderResourceByID(ResID id) {
		try
		{
			if (id < 10710000000)
			{
				return shaderRes->at(id - 10700000000);
			}
			else {
				return shaderRes->at(id - 10710000000);
			}
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
		
	}
	ObjectResource* fetchObjectResourceByID(ResID id) {
		try
		{
			if (id < 10210000000)
			{
				return objRes->at(id - 10200000000);
			}
			else if (id < 10220000000) {
				return objRes->at(id - 10210000000);
			}
			else {
				return objRes->at(id - 10220000000);
			}
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
		
	}
	MaterialResource* fetchMaterialResourceByID(ResID id) {
		try
		{
			return matRes->at(id - 10800000000);
		}
		catch (const std::exception&)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
		
	}

	RenderMeshResource* fetchRenderMeshResourceByID(ResID id) {
		try
		{
			return renderRes->at(id - 10300000000);
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
	}

	ColliderMeshResource* fetchColliderMeshResourceByID(ResID id) {
		try
		{
			return colRes->at(id - 10400000000);
		}
		catch (...)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
	}

	ImageResource* fetchImageResourceByID(ResID id) {
		try
		{
			return imageRes->at(id - 10600000000);
		}
		catch (const std::exception&)
		{
			throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
		}
	}

	bool resourceExists(ResID id) {
		for (int i = 0; i < masterList->size(); i++)
		{
			if (masterList->at(i) == id)
			{
				return true;
			}
		}
		return false;
	}

	// Initalizes CaseResource with resource handler and sets id and name!
	virtual void createCase(CaseResource* case_p) {};
	virtual void createScene(SceneResource* scn_p) {};
	virtual void createObject(ObjectResource* obj_p) {};
	virtual void createMaterial(MaterialResource* mat_p) {};
	virtual void createShader(ShaderResource* shader_p) {};
	virtual void createRenderMesh(RenderMeshResource* render_p) {};
	virtual void createColliderMesh(ColliderMeshResource* collider_p) {};
	virtual void createImage(ImageResource* image_p) {}


};


class Resource {
protected:
	bool init = false;
	ResID _id;
	virtual void setId(int index) {};

	std::string name;

	ResourceHandlerBuilderContext* resourceHandlerCxt;

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

protected: 
	// Scene cannot be duplicate in a Case.
	bool checkSceneDuplicate(ResID id) {
		for (int i = 0; i < Scenes->size(); i++)
		{
			if (Scenes->at(i) == id)
			{
				return true;
			}
		}
		return false;
	}


public:

	// RESOURCE SPECIFIC FUNCTIONS

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10000000000 + index;
	}


	CaseResource(ResourceHandlerBuilderContext* context, std::string name_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		context->createCase(this);
	}

	
	// CASE METHODS

	std::vector<ResID>* getScenesInCase() {
		return Scenes;
	}

	void addSceneToCase(ResID sceneID) {

		if (checkSceneDuplicate(sceneID))
		{
			throw ResourceHandlerIDError("Duplicate ID Not Supported");
		}
		else {
			if (sceneID >= 10100000000 && sceneID < 10200000000) {
				Scenes->push_back(sceneID);
			}
			else {
				throw ResourceHandlerIDError(("Invalid ID Entered. required : SceneResourceID , entered : " + std::to_string(sceneID)).c_str());
			}
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

private:
	int scnType; // Scene Type cannot change after being initalized.

protected:
	

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
		_id = 10100000000 + index + ((this->scnType) * 10000000); //  Assigns ID Based on SceneType
	}

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, int SceneType, Ogre::Vector3 position_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) {
		this->resourceHandlerCxt = context;

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

	int getSceneType() {
		return scnType;
	}

};


class ObjectResource : public Resource
{

private:
	PhysXType physXType;

protected:

	float mass;

	ResID renderMeshID;
	ResID colliderMeshID;

public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index + (this->physXType * 10000000);
	}
	
	ObjectResource(ResourceHandlerBuilderContext* context,std::string name_p, PhysXType objectType ) {
		this->resourceHandlerCxt = context;
		physXType = objectType;
		this->setName(name_p);

		context->createObject(this);
	}

	void setRenderMesh(ResID id) {
		if (10300000000 >= id && id < 10400000000)
		{
			if (this->resourceHandlerCxt->resourceExists(id)) {
				renderMeshID = id;
			}
			else {
				throw ResourceHandlerIDError("Resource is not init with Master List!");
			}
		}
	}
	void setColliderMesh(ResID id) {
		if (10400000000 >= id && id < 10500000000)
		{
			if (this->resourceHandlerCxt->resourceExists(id)) {
				colliderMeshID = id;
			}
			else {
				throw ResourceHandlerIDError("Resource is not init with Master List!");
			}
		}
	}

	void setMass(float mass_p) {
		mass = mass_p;
	}

	ResID getRenderMesh() { return renderMeshID; }
	ResID getColliderMesh() { return colliderMeshID; }

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

enum ShaderType {
	Vertex,
	Fragment
};

struct ShaderTexture
{
	std::string textureName;
	ResID texture;
	int texturePosition; // Position of texture in Shader

	ShaderTexture() {

	}

	ShaderTexture(ResID textureImgID) {
		texture = textureImgID;
	}
};

class ShaderResource : public Resource {
protected:
	std::string VertexShaderName; // file name of vertex shader
	std::string FragmentShaderName; // file name of fragment shader

	ShaderType shaderType;

	// These Parameters contain pre-saved values of Material and must be cross checked with Ogre's Shader parameters for consistancy
	std::vector<ShaderVar>* VertexParameters = new std::vector<ShaderVar>();
	std::vector<ShaderVar>* FragmentParameters = new std::vector<ShaderVar>();

public:
	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10700000000 + index + (shaderType * 10000000);
	}

	ShaderResource(ResourceHandlerBuilderContext* context, std::string name_p, ShaderType shaderType_p , std::string shaderFileName) {
		this->setName(name_p);
		this->resourceHandlerCxt = context;
		shaderType = shaderType_p;
		switch (shaderType)
		{
		case Vertex:
			VertexShaderName = shaderFileName;
			break;
		case Fragment:
			FragmentShaderName = shaderFileName;
			break;
		default:
			break;
		}

		context->createShader(this);
	}

	void addShaderParameter(ShaderVar variable) {
		switch (shaderType)
		{
		case Vertex:
			VertexParameters->push_back(variable);
			break;
		case Fragment:
			FragmentParameters->push_back(variable);
			break;
		default:
			break;
		}
	}



	std::vector<ShaderVar>* getShaderVars() {
		switch (shaderType)
		{
		case Vertex:
			return VertexParameters;
			break;
		case Fragment:
			return FragmentParameters;
			break;
		default:
			break;
		}
	}


	ShaderType getShaderType() {
		return shaderType;
	}
	
	std::string getShaderFileName() {
		switch (shaderType)
		{
		case Vertex:
			return VertexShaderName;
			break;
		case Fragment:
			return FragmentShaderName;
			break;
		default:
			break;
		}
	}

};

class MaterialResource : public Resource
{

protected:
	std::string materialName; // file name of material
	 
	// Shader Resource ID
	ResID VertexShaderResource; 
	ResID FragmentShaderResource; 
	
	// textures are stored in material and sent to ShaderResource for application
	std::vector<ShaderTexture>* Textures = new std::vector<ShaderTexture>(4);


public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10800000000 + index;
	}

	MaterialResource(ResourceHandlerBuilderContext* context,std::string name_p)
	{
		this->setName(name_p);
		this->resourceHandlerCxt = context;

		context->createMaterial(this);
	}

	void addVertexShader(ResID id) {
		if (id >= 10700000000 && id < 10710000000)
		{
			if (this->resourceHandlerCxt->resourceExists(id)) {
				VertexShaderResource = id;
			}
			else {
				throw ResourceHandlerIDError("ID does not exists in Master List");
			}
		}
		else {
			throw ResourceHandlerIDError("Invalid ID");
		}
	}

	void addFragmentShader(ResID id) {
		if (id >= 10710000000 && id < 10720000000)
		{
			if (this->resourceHandlerCxt->resourceExists(id)) {
				FragmentShaderResource = id;
			}
			else {
				throw ResourceHandlerIDError("ID does not exists in Master List");
			}
		}
		else {
			throw ResourceHandlerIDError("Invalid ID");
		}
	}

	void addDiffuseTexture(ShaderTexture diffuse_p) {
		Textures->at(0) = diffuse_p;
	}
	void addRoughnessTexture(ShaderTexture roughness_p) {
		Textures->at(1) = roughness_p;
	}
	void addParallaxTexture(ShaderTexture parallax_p) {
		Textures->at(2) = parallax_p;
	}
	void addNormalTexture(ShaderTexture normal_p) {
		Textures->at(3) = normal_p;
	}

	void addTexture(ResID Texture) {
		Textures->push_back(Texture);
	}

	void removeTextureByID(ResID Texture) {
		for (int i = 0; i < Textures->size(); i++)
		{
			if (Textures->at(i).texture == Texture)
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
	
public:

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10600000000 + index;

	}

	ImageResource(ResourceHandlerBuilderContext* context, std::filesystem::path imagePath_p) {
		imagePath = imagePath_p;

		this->setName(imagePath.filename().string());

		context->createImage(this);
	}

};



class RenderMeshResource : public Resource
{

protected:
	ResID material;

	std::string meshName;
	std::string meshFileName;
	
public:

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}

	RenderMeshResource(ResourceHandlerBuilderContext* context, std::string name_p, std::string meshName_p, std::string meshFileName_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		meshFileName = meshFileName_p;
		meshName = meshName_p;

		context->createRenderMesh(this);
	}

	std::string getMeshName() {
		return meshName;
	}

	std::string getMeshFileName() {
		return meshFileName;
	}

	MaterialResource* getMaterialResource() {
		return this->resourceHandlerCxt->fetchMaterialResourceByID(material);
	}

	ResID getMaterialID() {
		return material;
	}

	void setMaterial(ResID material_p) {
		material = material_p;
	}



};

class ColliderMeshResource : public Resource
{

protected:
	ResID material;

	std::string meshName;
	std::string meshFileName;
	
public:

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10400000000 + index;

	}

	ColliderMeshResource(ResourceHandlerBuilderContext* context, std::string name_p, std::string meshName_p, std::string meshFileName_p){
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		meshName = meshName_p;
		meshFileName = meshFileName_p;

		context->createColliderMesh(this);

	}

	std::string getMeshName() {
		return meshName;
	}

	std::string getMeshFileName() {
		return meshFileName;
	}

	MaterialResource* getMaterialResource() {
		return this->resourceHandlerCxt->fetchMaterialResourceByID(material);
	}

	ResID getMaterialID() {
		return material;
	}

	void setMaterial(ResID material_p) {
		material = material_p;
	}


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


	// Hide the constructor and destructor of the class
protected:
	ResourceHandler();
	~ResourceHandler() {};


public:

	// OVERHAUL FUNCTIONS
	// Checks if the folder structure and required files exists for Resource Tasks
	void checkFileStructure();

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





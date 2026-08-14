#pragma once

//Local
#include <GDHandler/ResourcePaths.h>
#include <ResourceHandler/ResourceReader.h>


// Third-party Headers
#include "yaml-cpp/yaml.h"

#include <monster/VulkanUtils.h>

// STL Headers
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>


// The Path storage in vectors are as follows
enum ResourcePaths {
	MasterList,
	Cases,
	Scenes,
	ScnObj,
	Objects,
	Shaders,
	ShaderVars,
	ShaderValues,
	MaterialPath,
	MaterialTexture,
	RenderMeshPath,
	ImagePath
};

namespace ResourceGroup {
	const std::vector < std::string > ResourceMasterGroups = {
	"Global",
		"Objects",
		"Shaders",
		"MaterialPath",
		"MaterialTexture",
		"RenderMeshPath",
		"ImagePath",
		"Font"
	};

	enum RESOURCE_MASTER_GROUP_INDEX { // MUST BE IN THE SAME ORDER AS ResourceMasterGroups
		GLOBAL,
		OBJECT,
		SHADER,
		MATERIAL_PATH,
		MATERIAL_TEXTURE,
		RENDER_MESH_PATH,
		IMAGE_PATH,
		FONT
	};
}

struct ResourceMasterGroup {

	ResourceMasterGroup(std::string name_p) {
		GroupName = name_p;
	}

	std::string GroupName;
	std::string loadPath; // name of the loadPath i.e. in ResourceLoader with containes the paths for all the contents fetched in this perticular Group
	int loadPathIndex;
	std::vector<std::filesystem::path>* ResourcePaths = new std::vector<std::filesystem::path>();

	// make load Paths as vector<> and imaplement String to path convertion in the ResourceLoader
};

typedef unsigned long long ResID;

enum PhysXType {
	Static,
	Dynamic,
	Kinematic
};


enum SceneType {
	STATIC,
	DYNAMIC,
	MESH
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

// Load Paths
// These paths are fetched when the application starts.
struct ResourceLoadPath {
	std::string pathGroupName;
	std::vector<std::string>* extensions;
	std::vector<std::string>* paths;

	std::string masterGroupName = "";
};

class CaseResource;
class SceneResource;
class ObjectResource;
class ShaderResource;
class MaterialResource;
class RenderMeshResource;
class ColliderMeshResource;
class ImageResource;


bool vectorPathContains(std::vector<std::filesystem::path>* checkVec, std::string file);

// MASTER RESOURCE CLASS. HANDLES STORAGE OF RESOURCE IDS and RESOURCES
class ResourceHandlerBuilderContext {

private:
	

protected:
	std::vector<ResID>* masterList = new std::vector<ResID>();


	int caseID = 0;
	int sceneID = 0;
	int objectID = 0;
	int shaderID = 0;
	int materialID = 0;
	int meshID = 0;
	int textureID = 0;


	void removeMasterListID(ResID id_p) {
		for (int i = 0; i < masterList->size(); i++)
		{
			if (masterList->at(i) == id_p)
			{
				masterList->erase(masterList->begin() + i);
				break;
			}
		}
	}

	

public:

	std::vector<ResID>* getMasterList() { return masterList; };


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

	int generateCaseID() {
		caseID += 1;
		return caseID;
	}
	int generateSceneID() {
		sceneID += 1;
		return sceneID;
	}
	int generateObjectID() {
		objectID += 1;
		return objectID;
	}
	int generateMeshID() {
		meshID += 1;
		return meshID;
	}
	int generateMaterialID() {
		materialID += 1;
		return materialID;
	}
	int generateShaderID() {
		shaderID += 1;
		return shaderID;
	}
	int generateTextureID() {
		textureID += 1;
		return textureID;
	}

	


};


class Resource {
protected:
	bool init = false;
	
	virtual void setId(int index) {};

	std::string name = "";

	ResourceHandlerBuilderContext* resourceHandlerCxt;

public:

	ResID _id;

	virtual void build(ResourceHandlerBuilderContext* context) {}

	void setName(std::string name_p) {
		name = name_p;
	}

	std::string getName() {
		return name;
	}


	ResID getId() { return _id; };

	virtual ~Resource() = default;

};


class CaseResource : public Resource {
private:

	std::string* caseFileName = nullptr;

	std::vector<ResID>* Scenes = new std::vector<ResID>();

protected:


public:

	// RESOURCE SPECIFIC FUNCTIONS
	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10000000000 + index;
	}


	CaseResource(ResourceHandlerBuilderContext* context, std::string name_p, std::string filename_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		setId(context->generateCaseID());
		context->AddIndexToMaster(getId());

		caseFileName = new std::string(filename_p);

	}

	// CASE FILE NAME CAN BE NULL! NOT ALL CASES ARE SAVED (THOUGHT THAT WAS OBVIOUS)
	std::string* getFileName() { return caseFileName; }


	// CASE METHODS

	std::vector<ResID>* getScenesIdInCase() {
		return Scenes;
	}

	virtual ~CaseResource() = default;


};

class SceneResource : public Resource
{

private:
	int scnType; // Scene Type cannot change after being initalized.

protected:

	float* position = new float[3]{};
	float* orientation = new float[4] {1.0,0.0,0.0,0.0};
	float* scale = new float[3] {1.0,1.0,1.0};


public:

	float* _getPosition() {
		
		return position;
	}

	float* _getOrientation() {
		return orientation;
	}

	float* _getScale() {
		return scale;
	}

	// UPDATES THE POSITION,ORIENTATION,SCALE WITH THE CORRESPONDING FLOAT*
	// PRIMARILY USED FOR IMGUI INPUT
	virtual void updatePosition() {}
	virtual void updateOrientation() {}
	virtual void updateScale() {}


	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10100000000 + index + ((this->scnType) * 10000000); //  Assigns ID Based on SceneType
	}

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, SceneType sceneType, glm::vec3  position_p, glm::vec4 orientation_p, glm::vec3 scale_p) {
		this->resourceHandlerCxt = context;

		scnType = sceneType;

		position[0] = position_p[0];
		position[1] = position_p[1];
		position[2] = position_p[2];
		orientation[0] = orientation_p[0];
		orientation[1] = orientation_p[1];
		orientation[2] = orientation_p[2];
		orientation[3] = orientation_p[3];
		scale[0] = scale_p[0];
		scale[1] = scale_p[1];
		scale[2] = scale_p[2];

		this->setName(name_p);

		setId(context->generateSceneID());
		context->AddIndexToMaster(getId());

	};

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, SceneType sceneType) {
		this->resourceHandlerCxt = context;

		scnType = sceneType;

		this->setName(name_p);

		setId(context->generateSceneID());
		context->AddIndexToMaster(getId());

	};

	int getSceneType() {
		return scnType;
	}

	~SceneResource() = default;

};


class ObjectResource : public Resource
{

private:
	PhysXType physXType;

protected:

	float mass = 0;

	std::string renderMeshName;

	std::filesystem::path meshFilePath;

public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index + (this->physXType * 10000000);
	}

	ObjectResource(ResourceHandlerBuilderContext* context, std::string name_p, PhysXType objectType,std::filesystem::path meshFilePath_p) {
		this->resourceHandlerCxt = context;
		physXType = objectType;
		this->setName(name_p);
		meshFilePath = meshFilePath_p;
		renderMeshName = meshFilePath.stem().string();
		setId(context->generateObjectID());
	}

	void setMass(float mass_p) {
		mass = mass_p;
	}

	PhysXType getPhysxType() { return physXType; }
	float getMass() { return mass; }
	
	const std::string _getMeshName() {
		return renderMeshName;
	}

	const std::filesystem::path getMeshFilePath() {
		return meshFilePath;
	}

	virtual ~ObjectResource() = default;

};


enum TextureMapType {
	DIFFUSE_MAP,
	AO_MAP,
	NORMAL_MAP,
	EDGE_MAP,
	HEIGHT_MAP,
	SMOOTHNESS_MAP,
	METALLIC_MAP
};

// hTexture hold the texture image path, loaded texture pointer and the texture position from which ShaderTexture can extract values
struct hTexture {
	std::filesystem::path imagePath;
	
};

// ShaderTexture does not hold the Texture it self but referes to it as multiple maps can be packed and extracted using this method
struct ShaderTexture
{
	std::string textureName = "";
	
	TextureMapType textureType;
	int texturePosition; // Position of texture in Shader

	float scale = 1.0f;
	
};

class ShaderResource : public Resource {
protected:
	std::string ShaderName; 
	std::string fileName;

	ShaderType shaderType;

	std::vector<ShaderTexture>* ShaderTextures = new std::vector<ShaderTexture>();

	// These Parameters contain pre-saved values of Material and must be cross checked with Ogre's Shader parameters for consistancy
	std::vector<ShaderVar>* ShaderParameters = new std::vector<ShaderVar>();

public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10700000000 + index + (shaderType * 10000000);
	}

	ShaderResource(ResourceHandlerBuilderContext* context, std::string name_p, ShaderType shaderType_p, std::string shaderFileName) {
		this->setName(name_p);
		this->resourceHandlerCxt = context;
		shaderType = shaderType_p;
		
		ShaderName = shaderFileName;
			
		setId(context->generateShaderID());

	}

	void addShaderParameter(ShaderVar variable) {
		ShaderParameters->push_back(variable);	
	}

	std::vector<ShaderVar>* getShaderVars() {
		return ShaderParameters;
	}

	std::vector<ShaderTexture>* getShaderTextures() {
		return ShaderTextures;
	}


	ShaderType getShaderType() {
		return shaderType;
	}

	std::string getShaderName() {
		return ShaderName;
	}

	std::string getShaderFileName() {
		return fileName;
	}

	~ShaderResource() = default;

};


class MaterialResource : public Resource
{

public:

	std::vector<ShaderTexture>* textures;


	bool wireframeMode = false;


	std::string materialFilePath; // file name of material


	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10800000000 + index;
	}


	MaterialResource(ResourceHandlerBuilderContext* context, std::string materialName, std::string materialFilePath_p) {
		this->setName(materialName);
		materialFilePath = materialFilePath_p;
		setId(context->generateMaterialID());
	};

	~MaterialResource() = default;

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

		setId(context->generateTextureID());
	}

	std::filesystem::path getImagePath() {
		return imagePath;
	}

};

class RenderMeshResource : public Resource
{

public:

	std::filesystem::path meshFile;

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}
	RenderMeshResource(ResourceHandlerBuilderContext* context, std::string meshName_p, std::filesystem::path meshFile_p) {
		this->resourceHandlerCxt = context;
		this->setName(meshName_p);
		meshFile = meshFile_p;
		setId(context->generateMeshID());
	}


	std::string getMeshName() { return name; }

	~RenderMeshResource() = default;

};

// NEW RESOURCES
namespace hRes {

	
	class Mesh {
	public:

		Mesh() {}

		std::vector<vulkanUtils::Vertex> vertices = std::vector<vulkanUtils::Vertex>();
		std::vector<uint16_t> indices = std::vector<uint16_t>();

		uint32_t vertexBufferIndex; // Most likly they'll both be the same but fuck it let's have em both.
		uint32_t indexBufferIndex;

		bool isMeshVkLoaded = false;

		// Used to set gropuing by vulkan to order rendering objects by the pipeline
		uint32_t graphicsPipelineIndex; // Default pipeline is used when this is null;

		uint32_t shaderIndex;

		Mesh(const Mesh& mesh) {
			this->graphicsPipelineIndex = mesh.graphicsPipelineIndex;
			this->indexBufferIndex = mesh.indexBufferIndex;
			this->vertexBufferIndex - mesh.vertexBufferIndex;
			this->indices = mesh.indices;
			this->shaderIndex = mesh.shaderIndex;
			this->vertices = mesh.vertices;
		}

	};


}






std::string convertFloatPtrToString(float* val_p, int size);
int convertStringToFloatPtr(std::string str_p,float* &output_p);


std::string convertShaderVarValueToStr(ShaderVar variable);
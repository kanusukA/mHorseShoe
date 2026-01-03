#pragma once

//Local
#include <Gui/GuiRegen.h>

// Third-party Headers
#include <Ogre.h>

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
	ColliderMeshPath,
	ImagePath
};

namespace ResourceLoaderEnums {

	enum ResourceLoadPaths {
		Objects,
		Shaders,
		MaterialPath,
		MaterialTexture,
		RenderMeshPath,
		ColliderMeshPath,
		ImagePath
	};

}

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
	GuiRegen* guiRegen; // used to update Gui Resources
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

	int caseIndex = 0;
	int sceneIndex = 0;
	int objectIndex = 0;
	int shaderIndex = 0;
	int matIndex = 0;
	int renderIndex = 0;
	int colliderIndex = 0;
	int imageIndex = 0;

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

		if (guiRegen)
		{
			guiRegen->regenScenes();
		}

	}
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

	void validateMasterList() {
		unsigned long long totalItems = caseRes->size() + scnRes->size() + objRes->size() +
			shaderRes->size() + matRes->size() + renderRes->size() + colRes->size() + imageRes->size();
		if (totalItems != masterList->size())
		{
			throw ResourceHandlerIDError("Master List is not valid!");
		}
	}

	int getCaseIndex() {
		return caseIndex;
	}
	int getSceneIndex() {
		return sceneIndex;
	}
	int getObjectIndex() {
		return objectIndex;
	}
	int getMaterialIndex() {
		return matIndex;
	}
	int getShaderIndex() {
		return shaderIndex;
	}
	int getRenderMeshIndex() {
		return renderIndex;
	}
	int getColliderMeshIndex() {
		return colliderIndex;
	}
	int getImageIndex() {
		return imageIndex;
	}

	void addCaseRes(CaseResource* case_p) {
		
		caseRes->push_back(case_p);
		validateMasterList();
		caseIndex += 1;
	}

	void addSceneRes(SceneResource* scn_p) {

		scnRes->push_back(scn_p);
		validateMasterList();
		sceneIndex += 1;

		if (guiRegen)
		{
			guiRegen->regenScenes();
		}
	}

	void addObjectRes(ObjectResource* obj_p) {

		objRes->push_back(obj_p);
		validateMasterList();
		objectIndex += 1;
	}

	void addShaderRes(ShaderResource* shader_p) {

		shaderRes->push_back(shader_p);
		validateMasterList();
		shaderIndex += 1;
	}

	void addMaterialRes(MaterialResource* mat_p) {

		matRes->push_back(mat_p);
		validateMasterList();
		matIndex += 1;
	}

	void addRenderMeshRes(RenderMeshResource* render_p) {
		renderRes->push_back(render_p);
		validateMasterList();
		renderIndex += 1;
	}

	void addColliderMeshRes(ColliderMeshResource* collider_p) {
		colRes->push_back(collider_p);
		validateMasterList();
		colliderIndex += 1;
	}

	void addImageRes(ImageResource* image_p) {
		imageRes->push_back(image_p);
		validateMasterList();
		imageIndex += 1;
	}

public:

	// Set GuiRegen to recieve updates on resource being added/changed/removed.
	void setGuiRegen(GuiRegen* guiRegen_p) {
		guiRegen = guiRegen_p;
	}

	std::vector<ResID>* getMasterList() { return masterList; };

	// CASE RESOURCE

	// Not a suggested method to fetch few cases. Try using getByID() insted. This method is only for GUI applications!
	std::vector<CaseResource*>* getAllCase() { return caseRes; };
	std::vector<SceneResource*>* getAllScenes() { return scnRes; };
	std::vector<ObjectResource*>* getAllObjects() { return objRes; };
	std::vector<MaterialResource*>* getAllMaterial() { return matRes; };
	std::vector<ShaderResource*>* getAllShader() { return shaderRes; };
	std::vector<RenderMeshResource*>* getAllRenderMesh() { return renderRes; };
	std::vector<ColliderMeshResource*>* getAllColliderMesh() { return colRes; };
	std::vector<ImageResource*>* getAllImages() { return imageRes; };

	SceneResource* fetchSceneResourceByID(ResID id) {
		try {
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

	void removeCase(CaseResource* case_p, ResID id) {
		for (int i = 0; i < caseRes->size(); i++)
		{
			if (caseRes->at(i) == case_p)
			{
				caseRes->erase(caseRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeScene(SceneResource* scn_p, ResID id) {
		for (int i = 0; i < scnRes->size(); i++)
		{
			if (scnRes->at(i) == scn_p)
			{
				scnRes->erase(scnRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeObject(ObjectResource* obj_p, ResID id) {
		for (int i = 0; i < objRes->size(); i++)
		{
			if (objRes->at(i) == obj_p)
			{
				objRes->erase(objRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeMaterial(MaterialResource* mat_p, ResID id) {
		for (int i = 0; i < matRes->size(); i++)
		{
			if (matRes->at(i) == mat_p)
			{
				matRes->erase(matRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeShader(ShaderResource* shader_p, ResID id) {
		for (int i = 0; i < shaderRes->size(); i++)
		{
			if (shaderRes->at(i) == shader_p)
			{
				shaderRes->erase(shaderRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeRenderMesh(RenderMeshResource* render_p, ResID id) {
		for (int i = 0; i < renderRes->size(); i++)
		{
			if (renderRes->at(i) == render_p)
			{
				renderRes->erase(renderRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeColliderMesh(ColliderMeshResource* collider_p, ResID id) {
		for (int i = 0; i < colRes->size(); i++)
		{
			if (colRes->at(i) == collider_p)
			{
				colRes->erase(colRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	};
	void removeImage(ImageResource* image_p, ResID id) {
		for (int i = 0; i < imageRes->size(); i++)
		{
			if (imageRes->at(i) == image_p)
			{
				imageRes->erase(imageRes->begin() + i);
				break;
			}
		}
		removeMasterListID(id);
	}


};


class Resource {
protected:
	bool init = false;
	ResID _id;
	virtual void setId(int index) {};

	std::string name = "";

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


class Case;

class CaseResource : public Resource {
private:

	Case* caseM;
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

	void _addSceneToCase(ResID sceneID) {

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

	void _removeSceneByID(ResID sceneID) {
		for (int i = 0; i < Scenes->size(); i++)
		{
			if (Scenes->at(i) == sceneID)
			{
				Scenes->erase(Scenes->begin() + i);
				break;
			}
		}
		
	}

	void _removeSceneByIndex(int index) {
		Scenes->erase(Scenes->begin() + index);

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


	CaseResource(ResourceHandlerBuilderContext* context,Case* case_p, std::string name_p) {
		caseM = case_p;
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		context->createCase(this);
	}


	// CASE METHODS

	std::vector<ResID>* getScenesIdInCase() {
		return Scenes;
	}

	~CaseResource() {
		resourceHandlerCxt->removeCase(this, _id);
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

	std::vector<ResID>* scenes = new std::vector<ResID>();
	std::vector<ResID>* objects = new std::vector<ResID>();


	void _addObject(ResID objectID) {
		if (objectID >= 10200000000 && objectID < 10300000000)
		{
			objects->push_back(objectID);
		}
		else {
			throw ResourceHandlerIDError(("Invalid ID Entered. required : SceneResourceID , entered : " + std::to_string(objectID)).c_str());
		}
	}

	void _attachScene(ResID sceneID) {
		if (sceneID >= 10100000000 && sceneID < 10200000000)
		{
			scenes->push_back(sceneID);
		}
		else {
			throw ResourceHandlerIDError(("Invalid ID Entered. required : SceneResourceID , entered : " + std::to_string(sceneID)).c_str());
		}
	}

	void _removeSceneById(ResID sceneID) {
		for (int i = 0; i < scenes->size(); i++)
		{
			if (scenes->at(i) == sceneID)
			{
				scenes->erase(scenes->begin() + i);
				break;
			}
		}
	}

	void _removeSceneByIndex(int index) {
		scenes->erase(scenes->begin() + index);
	}


	void _removeObjectById(ResID objectID) {
		for (int i = 0; i < objects->size(); i++)
		{
			if (objects->at(i) == objectID)
			{
				objects->erase(objects->begin() + i);
				break;
			}
		}
	}

	void _removeObjectByIndex(int index) {
		objects->erase(objects->begin() + index);
	}

public:

	virtual Ogre::Vector3 getPosition() {
		return position;
	}

	virtual Ogre::Quaternion getOrientation() {
		return Ogre::Quaternion(orientation[0], orientation[1],orientation[2],orientation[3]);
	}

	virtual Ogre::Vector3 getScale() {
		return scale;
	}

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10100000000 + index + ((this->scnType) * 10000000); //  Assigns ID Based on SceneType
	}

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, SceneType sceneType, Ogre::Vector3 position_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) {
		this->resourceHandlerCxt = context;

		scnType = sceneType;
		position = position_p;
		orientation = orientation_p;
		scale = scale_p;

		this->setName(name_p);

		context->createScene(this);

	};

	

	std::vector<ResID>* getAttachedScenesID() {
		return scenes;
	}

	std::vector<ResID>* getObjectsID() {
		return objects;
	}

	int getSceneType() {
		return scnType;
	}

	~SceneResource() {
		std::cout << "remove" << std::endl;
		resourceHandlerCxt->removeScene(this, _id);
	}

};

class Object;

class ObjectResource : public Resource
{

private:
	PhysXType physXType;

	Object* higherObj;

protected:

	float mass = 0;

	ResID renderMeshID = 0;
	ResID colliderMeshID = 0;

public:

	Object* getHigherClass() { return higherObj; }

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index + (this->physXType * 10000000);
	}

	ObjectResource(ResourceHandlerBuilderContext* context,Object* obj_p, std::string name_p, PhysXType objectType) {
		this->resourceHandlerCxt = context;
		higherObj = obj_p;
		physXType = objectType;
		this->setName(name_p);

		context->createObject(this);
	}

	ObjectResource(ResourceHandlerBuilderContext* context, std::string name_p, PhysXType objectType) {
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
	PhysXType getPhysxType() { return physXType; }
	float getMass() { return mass; }
	


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

	ShaderResource(ResourceHandlerBuilderContext* context, std::string name_p, ShaderType shaderType_p, std::string shaderFileName) {

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

class Material;

class MaterialResource : public Resource
{
private:
	Material* mat;

protected:
	std::string materialName; // file name of material

	// Shaders have to assigned manually!!!
	ResID VertexShaderResource;
	ResID FragmentShaderResource;

	// textures are stored in material and sent to ShaderResource for application
	std::vector<ShaderTexture>* Textures = new std::vector<ShaderTexture>(4);


public:

	Material* getHigherRef() {
		return mat;
	}

	ResID getVertexShader() {
		return VertexShaderResource;
	}
	ResID getFragmentShader() {
		return FragmentShaderResource;
	}

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10800000000 + index;
	}

	MaterialResource(ResourceHandlerBuilderContext* context,Material* mat_p, std::string name_p)
	{
		mat = mat_p;
		this->setName(name_p);
		this->resourceHandlerCxt = context;

		context->createMaterial(this);

	}

	void addVertexShader(ResID id) {
		// TODO TRY TO ADD SHADERS TO MATERIAL SCRIPT AS WELL 
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

	std::vector<ShaderTexture>* getTextures() {
		return Textures;
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

	std::filesystem::path getImagePath() {
		return imagePath;
	}

};

class RenderMesh;

class RenderMeshResource : public Resource
{

protected:
	ResID material;

	RenderMesh* renderMesh;

public:

	RenderMesh* getHigherClass() { return renderMesh; }

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}
	RenderMeshResource(ResourceHandlerBuilderContext* context,RenderMesh* renderMesh_p, std::string meshName_p) {
		this->resourceHandlerCxt = context;
		renderMesh = renderMesh_p;
		this->setName(meshName_p);


		context->createRenderMesh(this);
	}

	RenderMeshResource(ResourceHandlerBuilderContext* context, std::string meshName_p) {
		this->resourceHandlerCxt = context;
		this->setName(meshName_p);


		context->createRenderMesh(this);
	}

	MaterialResource* getMaterialResource() {
		return this->resourceHandlerCxt->fetchMaterialResourceByID(material);
	}

	ResID getMaterialID() {
		return material;
	}


	std::string getMeshName() { return name; }

	virtual void setMaterial(ResID material_p, Ogre::MaterialPtr mat_p) {};



};

class ColliderMeshResource : public Resource
{

protected:
	ResID material;

public:

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10400000000 + index;

	}

	ColliderMeshResource(ResourceHandlerBuilderContext* context, std::string meshName_p) {
		this->resourceHandlerCxt = context;
		this->setName(meshName_p);

		context->createColliderMesh(this);

	}

	MaterialResource* getMaterialResource() {
		return this->resourceHandlerCxt->fetchMaterialResourceByID(material);
	}

	ResID getMaterialID() {
		return material;
	}

	std::string getMeshName() { return name; }

	virtual void setMaterial(ResID material_p, Ogre::MaterialPtr mat_p) {};


};


std::string convertVec3ToString(Ogre::Vector3 vec3);
std::string convertVec4ToString(Ogre::Vector4 vec4);
Ogre::Vector4 convertStringToVec4(std::string str);
std::string convertShaderVarValueToStr(ShaderVar variable);
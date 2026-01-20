#pragma once

//Local
#include <GDHandler/ResourcePaths.h>
#include <ResourceHandler/ResourceReader.h>
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
	ImagePath
};

namespace ResourceLoaderEnums {

	enum ResourceLoadPaths {
		Objects,
		Shaders,
		MaterialPath,
		MaterialTexture,
		RenderMeshPath,
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
	

protected:
	std::vector<ResID>* masterList = new std::vector<ResID>();
	/*std::vector<CaseResource*>* caseRes = new std::vector<CaseResource*>();
	std::vector<SceneResource*>* scnRes = new std::vector<SceneResource*>();
	std::vector<ObjectResource*>* objRes = new std::vector<ObjectResource*>();


	std::vector<ShaderResource*>* shaderRes = new std::vector<ShaderResource*>();
	std::vector<std::unique_ptr<MaterialResource>>* matRes = new std::vector<std::unique_ptr<MaterialResource>>();
	std::vector<RenderMeshResource*>* renderRes = new std::vector<RenderMeshResource*>();
	std::vector<ColliderMeshResource*>* colRes = new std::vector<ColliderMeshResource*>();
	std::vector<ImageResource*>* imageRes = new std::vector<ImageResource*>();*/

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

	/*void validateMasterList() {
		unsigned long long totalItems = caseRes->size() + scnRes->size() + objRes->size() +
			shaderRes->size() + matRes->size() + renderRes->size() + colRes->size() + imageRes->size();
		if (totalItems != masterList->size())
		{
			throw ResourceHandlerIDError("Master List is not valid!");
		}
	}*/

	

// 	void addCaseRes(CaseResource* case_p) {
// 		
// 		caseRes->push_back(case_p);
// 		validateMasterList();
// 		caseIndex += 1;
// 	}
// 
// 	void addSceneRes(SceneResource* scn_p) {
// 
// 		scnRes->push_back(scn_p);
// 		validateMasterList();
// 		sceneIndex += 1;
// 
// 		if (guiRegen)
// 		{
// 			guiRegen->regenScenes();
// 		}
// 	}
// 
// 	void addObjectRes(ObjectResource* obj_p) {
// 
// 		objRes->push_back(obj_p);
// 		validateMasterList();
// 		objectIndex += 1;
// 	}
// 
// 	void addShaderRes(ShaderResource* shader_p) {
// 
// 		shaderRes->push_back(shader_p);
// 		validateMasterList();
// 		shaderIndex += 1;
// 	}
// 
// 	void addMaterialRes(std::unique_ptr<MaterialResource> uniqueMaterial) {
// 		matRes->push_back(std::move(uniqueMaterial));
// 		validateMasterList();
// 		matIndex += 1;
// 	}
// 
// 	void addRenderMeshRes(RenderMeshResource* render_p) {
// 		renderRes->push_back(render_p);
// 		validateMasterList();
// 		renderIndex += 1;
// 	}
// 
// 	void addColliderMeshRes(ColliderMeshResource* collider_p) {
// 		colRes->push_back(collider_p);
// 		validateMasterList();
// 		colliderIndex += 1;
// 	}
// 
// 	void addImageRes(ImageResource* image_p) {
// 		imageRes->push_back(image_p);
// 		validateMasterList();
// 		imageIndex += 1;
// 	}

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

	// CASE RESOURCE

	// Not a suggested method to fetch few cases. Try using getByID() insted. This method is only for GUI applications!
	//std::vector<CaseResource*>* getAllCase() { return caseRes; };
	//std::vector<SceneResource*>* getAllScenes() { return scnRes; };
	//std::vector<ObjectResource*>* getAllObjects() { return objRes; };
	//std::vector<std::unique_ptr<MaterialResource>>* getAllMaterial() { return matRes; };
	//std::vector<ShaderResource*>* getAllShader() { return shaderRes; };
	//std::vector<RenderMeshResource*>* getAllRenderMesh() { return renderRes; };
	//std::vector<ColliderMeshResource*>* getAllColliderMesh() { return colRes; };
	//std::vector<ImageResource*>* getAllImages() { return imageRes; };

	//SceneResource* fetchSceneResourceByID(ResID id) {
	//	try {
	//		if (id < 10110000000)
	//		{
	//			return scnRes->at(id - 10100000000);
	//		}
	//		else if (id < 10120000000)
	//		{
	//			return scnRes->at(id - 10110000000);
	//		}
	//		else {
	//			return scnRes->at(id - 10120000000);
	//		}

	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}

	//}
	//CaseResource* fetchCaseResourceByID(ResID id) {
	//	try
	//	{
	//		return caseRes->at(id - 10000000000);
	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}

	//}
	//ShaderResource* fetchShaderResourceByID(ResID id) {
	//	try
	//	{
	//		if (id < 10710000000)
	//		{
	//			return shaderRes->at(id - 10700000000);
	//		}
	//		else {
	//			return shaderRes->at(id - 10710000000);
	//		}
	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}

	//}
	//ObjectResource* fetchObjectResourceByID(ResID id) {
	//	try
	//	{
	//		if (id < 10210000000)
	//		{
	//			return objRes->at(id - 10200000000);
	//		}
	//		else if (id < 10220000000) {
	//			return objRes->at(id - 10210000000);
	//		}
	//		else {
	//			return objRes->at(id - 10220000000);
	//		}
	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}

	//}
	///*MaterialResource* fetchMaterialResourceByID(ResID id) {
	//	try
	//	{
	//		return matRes->at(id - 10800000000);
	//	}
	//	catch (const std::exception&)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}

	//}*/

	//RenderMeshResource* fetchRenderMeshResourceByID(ResID id) {
	//	try
	//	{
	//		return renderRes->at(id - 10300000000);
	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}
	//}

	//ColliderMeshResource* fetchColliderMeshResourceByID(ResID id) {
	//	try
	//	{
	//		return colRes->at(id - 10400000000);
	//	}
	//	catch (...)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}
	//}

	//ImageResource* fetchImageResourceByID(ResID id) {
	//	try
	//	{
	//		return imageRes->at(id - 10600000000);
	//	}
	//	catch (const std::exception&)
	//	{
	//		throw ResourceHandlerIDError(("Resource Does not Exist , id : " + std::to_string(id)).c_str());
	//	}
	//}

	//bool resourceExists(ResID id) {
	//	for (int i = 0; i < masterList->size(); i++)
	//	{
	//		if (masterList->at(i) == id)
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	//// Initalizes CaseResource with resource handler and sets id and name!
	//virtual void createCase(CaseResource* case_p) {};
	//virtual void createScene(SceneResource* scn_p) {};
	//virtual void createObject(ObjectResource* obj_p) {};
	//virtual void createMaterial(MaterialResource* mat_p) {};
	//virtual void createShader(ShaderResource* shader_p) {};
	//virtual void createRenderMesh(RenderMeshResource* render_p) {};
	//virtual void createColliderMesh(ColliderMeshResource* collider_p) {};
	//virtual void createImage(ImageResource* image_p) {}

	//void removeCase(CaseResource* case_p, ResID id) {
	//	for (int i = 0; i < caseRes->size(); i++)
	//	{
	//		if (caseRes->at(i) == case_p)
	//		{
	//			caseRes->erase(caseRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeScene(SceneResource* scn_p, ResID id) {
	//	for (int i = 0; i < scnRes->size(); i++)
	//	{
	//		if (scnRes->at(i) == scn_p)
	//		{
	//			scnRes->erase(scnRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeObject(ObjectResource* obj_p, ResID id) {
	//	for (int i = 0; i < objRes->size(); i++)
	//	{
	//		if (objRes->at(i) == obj_p)
	//		{
	//			objRes->erase(objRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeMaterial(ResID id) {
	//	for (int i = 0; i < matRes->size(); i++)
	//	{
	//		if (matRes->at(i)->_id == id)
	//		{
	//			matRes->erase(matRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeShader(ShaderResource* shader_p, ResID id) {
	//	for (int i = 0; i < shaderRes->size(); i++)
	//	{
	//		if (shaderRes->at(i) == shader_p)
	//		{
	//			shaderRes->erase(shaderRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeRenderMesh(RenderMeshResource* render_p, ResID id) {
	//	for (int i = 0; i < renderRes->size(); i++)
	//	{
	//		if (renderRes->at(i) == render_p)
	//		{
	//			renderRes->erase(renderRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeColliderMesh(ColliderMeshResource* collider_p, ResID id) {
	//	for (int i = 0; i < colRes->size(); i++)
	//	{
	//		if (colRes->at(i) == collider_p)
	//		{
	//			colRes->erase(colRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//};
	//void removeImage(ImageResource* image_p, ResID id) {
	//	for (int i = 0; i < imageRes->size(); i++)
	//	{
	//		if (imageRes->at(i) == image_p)
	//		{
	//			imageRes->erase(imageRes->begin() + i);
	//			break;
	//		}
	//	}
	//	removeMasterListID(id);
	//}


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

};


class CaseResource : public Resource {
private:

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


	CaseResource(ResourceHandlerBuilderContext* context, std::string name_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		setId(context->generateCaseID());
		context->AddIndexToMaster(getId());

	}


	// CASE METHODS

	std::vector<ResID>* getScenesIdInCase() {
		return Scenes;
	}


};

class SceneResource : public Resource
{

private:
	int scnType; // Scene Type cannot change after being initalized.

protected:

	float* position = new float[3]{};
	float* orientation = new float[4] {};
	float* scale = new float[3] {1,1,1};


public:

	float* _getPosition() {
		//std::cout << "position : " << position[0] << " " << position[1] << " " << position[2] << std::endl;
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

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p, SceneType sceneType, Ogre::Vector3 position_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) {
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

	int getSceneType() {
		return scnType;
	}

};


class ObjectResource : public Resource
{

private:
	PhysXType physXType;

protected:

	float mass = 0;


public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index + (this->physXType * 10000000);
	}

	ObjectResource(ResourceHandlerBuilderContext* context, std::string name_p, PhysXType objectType) {
		this->resourceHandlerCxt = context;
		physXType = objectType;
		this->setName(name_p);

		setId(context->generateObjectID());
	}

	void setMass(float mass_p) {
		mass = mass_p;
	}

	PhysXType getPhysxType() { return physXType; }
	float getMass() { return mass; }
	


};



struct ShaderTexture
{
	std::string textureName = "";
	ResID texture = 0;
	int texturePosition; // Position of texture in Shader

	ShaderTexture() {

	}

	ShaderTexture(ResID textureImgID) {
		texture = textureImgID;
	}
};

class ShaderResource : public Resource {
protected:
	std::string ShaderName; 
	std::string fileName;

	ShaderType shaderType;

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


	ShaderType getShaderType() {
		return shaderType;
	}

	std::string getShaderName() {
		return ShaderName;
	}

	std::string getShaderFileName() {
		return fileName;
	}

};


class MaterialResource : public Resource
{

public:

	std::string materialName; // file name of material


	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10800000000 + index;
	}


	MaterialResource(ResourceHandlerBuilderContext* context, std::string materialName_p) {
		this->setName(materialName_p);
		materialName = materialName_p;
		setId(context->generateMaterialID());
	};


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

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}
	RenderMeshResource(ResourceHandlerBuilderContext* context, std::string meshName_p) {
		this->resourceHandlerCxt = context;
		this->setName(meshName_p);

		setId(context->generateMeshID());
	}


	std::string getMeshName() { return name; }



};


std::string convertVec3ToString(Ogre::Vector3 vec3);
std::string convertVec4ToString(Ogre::Vector4 vec4);
Ogre::Vector4 convertStringToVec4(std::string str);
std::string convertFloatPtrToString(float* val_p, int size);
int convertStringToFloatPtr(std::string str_p,float* &output_p);
Ogre::Vector3 convertFloatPtrToVec3(float* val_p);
Ogre::Vector4 convertFloatPtrToVec4(float* val_p);

std::string convertShaderVarValueToStr(ShaderVar variable);
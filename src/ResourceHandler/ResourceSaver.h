#pragma once


//Local Header
#include <ResourceHandler/ResourceObjects.h>

// Third Headers
#include "SimpleIni.h"

//STL Headers
#include <fstream>





// A Fragment of ResourceHandler
// NOTE
// Each piece of data such as a CaseResource, which has other Resource references, are not stored with them i.e. These Resource only store the references/ID of other resources. 
// Make sure referenced Resources are also saved separately as it can lead to Load Errors!
class ResourceSaver
{
private:

	std::string saveLocation;

	CSimpleIniA* ini;

	// Used to update File
	bool loadIniFile(std::string filename);


	// Transfer Data From memory to Disk
	void saveIni(std::string filename);
	void resetIni();

	void saveIniFile(std::string filename, std::string section, std::string key, std::string value);

	void saveMasterList(std::string instanceName, std::vector<ResID>* master_p, std::string path);

	void saveCase(CaseResource* case_p);

	void saveScene(SceneResource* scene_p);
	void saveScnObj(std::string sectionName, ResID objectID);

	void saveMaterial(MaterialResource* mat_p);
	void saveMaterialTexture(std::string sectionName, ResID textureID);

	void saveRenderMesh(RenderMeshResource* renderMesh_p);
	void saveColliderMesh(ColliderMeshResource* colliderMesh_p);

	void saveShader(ShaderResource* shader_p);
	void saveShaderVar(std::string sectionName, std::string varName);
	// Here section is the ShaderID and key can be set to ShaderVarName. different shader can shader same parameter name.
	void saveShaderValue(std::string sectionName,std::string key, std::string value);

	void saveImage(ImageResource* image_p);
	void saveObject(ObjectResource* obj_p);

protected:

	void saveCases(std::vector<CaseResource*>* case_res, std::string caseInipath);
	void saveScenes(std::vector<SceneResource*>* scene_res, std::string scnInipath, std::string scnObjInipath);
	void saveMaterials(std::vector<MaterialResource*>* mast_res, std::string matIniPath, std::string matTexIniPath);
	void saveObjects(std::vector<ObjectResource*>* obj_res, std::string objIniPath);

	void saveColliderMesh(std::vector<ColliderMeshResource*>* col_res, std::string colIniPath);
	void saveRenderMesh(std::vector<RenderMeshResource*>* render_res, std::string renderIniPath);

	void saveShaders(std::vector<ShaderResource*>* shader_res, std::string shaderIniPath, std::string shaderVarIniPath, std::string shaderValueIniPath);

	void saveImages(std::vector<ImageResource*>* image_res, std::string imageIniPath);
	

public:

	ResourceSaver(CSimpleIniA* ini_p, std::string savelocation_p) {
		saveLocation = savelocation_p;
		ini = ini_p;
	}


};


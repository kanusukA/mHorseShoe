#pragma once


//Local Header
#include <ResourceHandler/ResourceObjects.h>
#include <timer/glock.h>

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
public:

	std::ofstream outStreamFile;

	std::string saveLocation;

	CSimpleIniA* ini;

	// Used to update File
	bool loadIniFile(std::string filename);


	// Transfer Data From memory to Disk
	void saveIni(std::string filename);
	void resetIni();

	void saveIniFile(std::string filename, std::string section, std::string key, std::string value);

	bool openSaveFile(std::string filepath);
	void writeToSaveFile(const char* data);
	void closeSaveFile();

	// Runs OpenSaveFile(). make sure to close it.
	// returns the filename. this can be passed on to same content in the same file!
	std::string saveCase(CaseResource* case_p);

	void saveScene(SceneResource* scene_p, YAML::Emitter& out);
	void saveScnObj(std::string sectionName, ResID objectID);

	void saveMaterial(MaterialResource* mat_p, ShaderResource* vert_p, ShaderResource* frag_p,YAML::Emitter& out);
	void saveMaterialTexture(std::string sectionName, std::string textureName, int pos);

	void saveRenderMesh(RenderMeshResource* renderMesh_p);
	void saveColliderMesh(ColliderMeshResource* colliderMesh_p);

	void saveShader(ShaderResource* shader_p, ShaderType type, ResID attachedToObject);
	void saveShaderVar(std::string sectionName, std::string varName, std::string type);
	// Here section is the ShaderID and key can be set to ShaderVarName. different shader can shader same parameter name.
	void saveShaderValue(std::string sectionName,std::string key, std::string value);

	void saveImage(ImageResource* image_p);
	void saveObject(ObjectResource* obj_p, YAML::Emitter& out);

	// LOAD PATHS
	void saveResourceLoadPaths(std::vector<ResourceLoadPath>* loadPaths, std::string path);



	void saveMasterList(std::string instanceName, std::vector<ResID>* master_p, std::string path, bool overwrite = true);

	void saveCases(std::vector<CaseResource*>* case_res, std::string caseInipath, bool overwrite = true);
	void saveScenes(std::vector<SceneResource*>* scene_res, std::string scnInipath, std::string scnObjInipath, bool overwrite = true);
	void saveMaterials(std::vector<MaterialResource*>* mast_res, std::string matIniPath, std::string matTexIniPath, bool overwrite = true);
	void saveObjects(std::vector<ObjectResource*>* obj_res, std::string objIniPath, bool overwrite = true);

	void saveColliderMesh(std::vector<ColliderMeshResource*>* col_res, std::string colIniPath, bool overwrite = true);
	void saveRenderMesh(std::vector<RenderMeshResource*>* render_res, std::string renderIniPath, bool overwrite = true);

	void saveShaders(std::vector<ShaderResource*>* shader_res, std::string shaderIniPath, std::string shaderVarIniPath, std::string shaderValueIniPath, bool overwrite = true);

	void saveImages(std::vector<ImageResource*>* image_res, std::string imageIniPath, bool overwrite = true);
	

public:

	void initResourceSaver(CSimpleIniA* ini_p, std::string savelocation_p) {
		saveLocation = savelocation_p;
		ini = ini_p;
	}


};


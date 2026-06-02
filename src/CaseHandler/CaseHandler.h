#pragma once

#include <GDHandler/objects.h>





// setup imports

// Manages Case. i.e all the Scenes in a single save file
// Also manages the integration of ResourceScenes and Ogre::Scenes
class CaseHandler : public GDBuilderContext{

private:


protected:

	// Ogre::Scenes must remain consistant with ResourceHandler.
	void checkIntegrity();


public:
	StuffHandler* stuffHandler;
	Ogre::SceneManager* oScnManager;

	//std::shared_ptr<std::string> defaultCase = std::make_shared<std::string>();

	std::vector<std::shared_ptr<Case>>* caseVec = new std::vector<std::shared_ptr<Case>>();




	// DEPEDENT RESOURCE VECTOR
	// These resources are used to create copy of themselves to be used by other resources in a lifecycle aware manner. (hopefully)

	std::vector<std::unique_ptr<RenderMesh>>* meshVec = new std::vector<std::unique_ptr<RenderMesh>>();
	std::vector<std::unique_ptr<Material>>* materialVec = new std::vector<std::unique_ptr<Material>>();
	

	// CASE FUNCTIONS
	CaseHandler(Monster* monster) : GDBuilderContext(ResourceHandler::GetInstance(), monster) {
		//stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		stuffHandler = new StuffHandler(monster);
		oScnManager = monster->oScnManager;

	};

	// Used to start CaseHandler with a predefined case as the program starts
	CaseHandler(Monster* monster, Case* case_p) : GDBuilderContext(ResourceHandler::GetInstance(), monster) {

		//stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		stuffHandler = new StuffHandler(monster);
		oScnManager = monster->oScnManager;

	};

	bool resourceExists(std::string resourceName);


	std::vector<std::shared_ptr<Case>>* fetchAllCases() { return caseVec; }


	std::weak_ptr<Case> CreateCase(std::string caseName_p);
	std::weak_ptr<Case> CreateCase(std::string caseName_p,std::string filename_p);
	Scene* CreateScene(std::string scnName, SceneType scnType, Ogre::SceneNode* parentNode_p) override;
	Scene* CreateScene(std::string scnName, SceneType scnType) override;
	Object* CreateObject(std::string objName_p , std::filesystem::path meshPath_p, PhysXType type) override;

	// DEPENDENT RESOURCE
	Ogre::MeshPtr fetchMeshByName(std::filesystem::path meshPath_p) override;
	Ogre::MeshPtr fetchMeshById(ResID meahID_p) override;
	Ogre::TexturePtr fetchImageByName(std::filesystem::path imagePath_p);


	//RenderMesh* CreateRenderMesh(std::filesystem::path path_p);
	Material* CreateMaterial( std::filesystem::path materialPath_p, std::string materialName);

	// old method
	//RenderMesh* CreateRenderMesh(std::string meshName_p);

	//ColliderMesh* CreateColliderMesh(std::string MeshName_p);
	//Shader* CreateShader(Ogre::MaterialPtr mat_p, ShaderType type); Shaders are created by Materials themselves!
	// MaterialName must be valid with Ogre::Material before Calling CreateMaterial function.
	
	Image* CreateImage(std::filesystem::path filePath_p);

	void loadSavedResource();

	// Loading Functions
	void loadCase(std::filesystem::path yamlFilePath);

	// RETURES THE SAVED CASE FILENAME! WILL RETURN EMPTY STRING IS NON IS SAVED.
	std::string loadDefaultCase();

	// THE DEFAULT FILENAME MUST MATCH THE FILENAME INSIDE THE SOURCE DIR / DATA FOLDER
	void saveDefaultCase(std::string saveYmlFileName);
	void saveAll();
	void saveScenes(const std::vector<std::shared_ptr<Scene>>* scenes);
	void saveObjects(const std::vector<std::shared_ptr<Object>>* objects, YAML::Emitter& out);
	/*void saveMaterial();
	void saveShader();*/

	void unload() {};

};

class CaseErrorHandler : public std::exception {

public:

//	CaseErrorHandler(const char* what) : std::exception(what) {
//	}

};

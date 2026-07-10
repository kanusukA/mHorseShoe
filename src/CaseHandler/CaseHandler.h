#pragma once

#include <GDHandler/objects.h>





// setup imports

// Manages Case. i.e all the Scenes in a single save file
// Also manages the integration of ResourceScenes and Ogre::Scenes
class CaseHandler : public GDBuilderContext{

private:


protected:

	// CREATE A CHECK INTEGRITY FUNCTION FOR SAVE FILES THAT CAN PRESCAN IF THE FILES REQUIRED BY THE SAVE FILE EXISTS!
	//void checkIntegrity();


public:

	Ogre::SceneManager* oScnManager;

	//std::shared_ptr<std::string> defaultCase = std::make_shared<std::string>();

	std::vector<std::shared_ptr<Case>>* caseVec = new std::vector<std::shared_ptr<Case>>();


	// MAIN SELECTED CASE
	// TO SET THIS CASE ALWAYS USE THE LOAD CASE FUNCTION TO AVOID CREATING A CASE WITH INVALID SCENES.
	static std::unique_ptr<Case> selectedCase; 


	// DEPEDENT RESOURCE VECTOR
	// These resources are used to create copy of themselves to be used by other resources in a lifecycle aware manner. (hopefully)

	std::vector<std::unique_ptr<RenderMesh>>* meshVec = new std::vector<std::unique_ptr<RenderMesh>>();
	std::vector<std::unique_ptr<Material>>* materialVec = new std::vector<std::unique_ptr<Material>>();
	

	// CASE FUNCTIONS
	CaseHandler(Monster* monster) : GDBuilderContext(ResourceHandler::GetInstance(), monster) {
		//stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		//oScnManager = monster->oScnManager;

	};

	// Used to start CaseHandler with a predefined case as the program starts
	CaseHandler(Monster* monster, Case* case_p) : GDBuilderContext(ResourceHandler::GetInstance(), monster) {

		//stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		oScnManager = monster->oScnManager;

	};

	bool resourceExists(std::string resourceName);


	std::vector<std::shared_ptr<Case>>* fetchAllCases() { return caseVec; }


	Case* CreateCase(std::string caseName_p);
	Case* CreateCase(std::string caseName_p,std::string filename_p);
	Scene* CreateScene(std::string scnName, SceneType scnType, Ogre::SceneNode* parentNode_p) override;
	Scene* CreateScene(std::string scnName, SceneType scnType) override;
	Object* CreateObject(std::string objName_p , std::filesystem::path meshPath_p, PhysXType type) override;

	// DEPENDENT RESOURCE
	Ogre::MeshPtr fetchMeshByName(std::filesystem::path meshPath_p) override;
	Ogre::MeshPtr fetchMeshById(ResID meahID_p) override;
	Ogre::TexturePtr fetchImageByName(std::filesystem::path imagePath_p);


	//RenderMesh* CreateRenderMesh(std::filesystem::path path_p);
	Material* CreateMaterial( std::filesystem::path materialPath_p, std::string materialName);

	Image* CreateImage(std::filesystem::path filePath_p);

	// Loading Functions
	void loadCase(std::filesystem::path yamlFilePath);

	// RETURNS THE SAVED CASE FILENAME! WILL RETURN EMPTY STRING IS NONE IS SAVED.
	std::string loadDefaultCase();

	// THE DEFAULT FILENAME MUST MATCH THE FILENAME INSIDE THE SOURCE DIR / DATA FOLDER
	void saveDefaultCase(std::string saveYmlFileName);
	void saveAll();
	void saveScenes(const std::vector<std::shared_ptr<Scene>>* scenes);
	void saveObjects(const std::vector<std::shared_ptr<Object>>* objects, YAML::Emitter& out);

	void unload();

};

class CaseErrorHandler : public std::exception {

public:

//	CaseErrorHandler(const char* what) : std::exception(what) {
//	}

};

#pragma once

#include <GDHandler/objects.h>





// setup imports

// Manages Case. i.e all the Scenes in a single save file
// Also manages the integration of ResourceScenes and Ogre::Scenes
class CaseHandler : public GDBuilderContext, public MaterialManager {

private:
	


protected:

	// Ogre::Scenes must remain consistant with ResourceHandler.
	void checkIntegrity();



public:

	std::unique_ptr<StuffHandler> stuffHandler;
	Ogre::SceneManager* oScnManager;


	std::vector<std::unique_ptr<Case>>* caseVec = new std::vector<std::unique_ptr<Case>>();
	int selCase;

	// DEPEDENT RESOURCE VECTOR
	// These resources are used to create copy of themselves to be used by other resources in a lifecycle aware manner. (hopefully)

	std::vector<std::unique_ptr<RenderMesh>>* meshVec = new std::vector<std::unique_ptr<RenderMesh>>();
	

	// CASE FUNCTIONS

	CaseHandler(Monster* monster, Kint * kint) : GDBuilderContext(ResourceHandler::GetInstance(), monster,kint) {
		stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		oScnManager = monster->oScnManager;

	};
	// Used to start CaseHandler with a predefined case as the program starts
	CaseHandler(Monster* monster, Kint* kint, Case* case_p) : GDBuilderContext(ResourceHandler::GetInstance(), monster, kint) {

		stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		
		oScnManager = monster->oScnManager;

	};

	void setCaseName(std::string caseName) {
		caseVec->at(selCase)->setName(caseName);
	}


	std::vector<std::unique_ptr<Case>>* fetchAllCases() { return caseVec; }


	void CreateCase(std::string caseName_p);
	Scene* CreateScene(std::string scnName, SceneType scnType) override;
	Object* CreateObject(std::string objName_p , std::filesystem::path meshPath_p, PhysXType type);

	// DEPENDENT RESOURCE
	Ogre::MeshPtr fetchMeshByName(std::filesystem::path meshPath_p) override;
	Ogre::MeshPtr fetchMeshById(ResID meahID_p) override;



	// old method
	//RenderMesh* CreateRenderMesh(std::string meshName_p);

	RenderMesh* CreateRenderMesh(std::filesystem::path path_p);

	ResID CreateMaterialResource(std::filesystem::path path_p);

	ColliderMesh* CreateColliderMesh(std::string MeshName_p);
	Shader* CreateShader(Ogre::MaterialPtr mat_p, ShaderType type);
	// MaterialName must be valid with Ogre::Material before Calling CreateMaterial function.
	
	Image* CreateImage(std::filesystem::path filePath_p);


	void loadSavedResource();

	// GD_CONTEXT FUNCTIONS
	Material* getMaterial(std::filesystem::path materialName);



};

class CaseErrorHandler : public std::exception {

public:

	CaseErrorHandler(const char* what) : std::exception(what) {
	}

};
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

	Case* currentCase;
	ObjectPtr selectedObject = ObjectPtr();


	std::vector<Case*>* cases = new std::vector<Case*>();

	// CASE FUNCTIONS

	CaseHandler(Monster* monster, Kint * kint) : GDBuilderContext(ResourceHandler::GetInstance(), monster,kint) {
		stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		oScnManager = monster->oScnManager;

		// Set Case manually
		
		// TODO Check Resource Loader for case
		currentCase = CreateCase("Unnamed case");
		

	};
	// Used to start CaseHandler with a predefined case as the program starts
	CaseHandler(Monster* monster, Kint* kint, Case* case_p) : GDBuilderContext(ResourceHandler::GetInstance(), monster, kint) {

		stuffHandler = std::make_unique<StuffHandler>(monster, kint);
		
		oScnManager = monster->oScnManager;
		currentCase = case_p;

		//this->loadSavedResource();

	};

	void setCaseName(std::string caseName) {
		currentCase->setName(caseName);
	}

	Case* getCurrentCase() {
		return currentCase;
	}

	void setSelectedObject(Object* obj_p) {
		selectedObject.set(obj_p);
	}
	ObjectPtr* getSelectedObject() {
		return &selectedObject;
	}

	std::vector<Case*>* getAllCases() { return cases; }


	Case* CreateCase(std::string caseName_p);
	Scene* CreateScene(SceneType scnType, std::string scnName);
	Scene* CreateSceneAttachToCase(SceneType scnType, std::string scnName);
	Object* CreateObject(std::string objectName_p, RenderMesh* renderMesh_p, PhysXType type);

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
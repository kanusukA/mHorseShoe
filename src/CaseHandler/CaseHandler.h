#pragma once

#include <GDHandler/objects.h>


// setup imports

// Manages Case. i.e all the Scenes in a single save file
// Also manages the integration of ResourceScenes and Ogre::Scenes
class CaseHandler {

private:
	GDBuilderContext* builderCxt;
	StuffHandler* stuffHandler;
	Ogre::SceneManager* oScnManager;

	Case* currentCase;


protected:

	// Ogre::Scenes must remain consistant with ResourceHandler.
	void checkIntegrity();



public:

	// CASE FUNCTIONS

	CaseHandler(GDBuilderContext* builderCxt_p,StuffHandler* stuffHandler_p, Ogre::SceneManager* oScnManager_p) {
		builderCxt = builderCxt_p;
		stuffHandler = stuffHandler_p;
		oScnManager = oScnManager_p;

		// Set Case manually
		
		// TODO Check Resource Loader for case
		currentCase = new Case(builderCxt_p,"Unnamed Case");

	};
	// Used to start CaseHandler with a predefined case as the program starts
	CaseHandler(StuffHandler* stuffHandler_p, Ogre::SceneManager* oScnManager_p, Case* case_p) {

		stuffHandler = stuffHandler_p;
		oScnManager = oScnManager_p;
		currentCase = case_p;

	};

	void setCaseName(std::string caseName) {
		currentCase->setName(caseName);
	}

	Case* getCurrentCase() {
		return currentCase;
	}
	// Add an update method for converting ResID to sceneType!!

	// SCENE FUNCTIONS
	std::vector<SceneResource*> getDynamicScenes() {
		
	}

	Scene* CreateScene(SceneType scnType, std::string scnName);
	Object* CreateObject(std::string objectName_p, PhysXType type);
	RenderMesh* CreateRenderMesh(std::string meshName_p);
	ColliderMesh* CreateColliderMesh(std::string MeshName_p);
	Shader* CreateShader(Ogre::MaterialPtr mat_p, ShaderType type);
	Material* CreateMaterial(std::string materialName_p);
	Image* CreateImage(std::filesystem::path filePath_p);



};

class CaseErrorHandler : public std::exception {

public:

	CaseErrorHandler(const char* what) : std::exception(what) {
	}

};
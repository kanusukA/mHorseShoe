#pragma once

#include <GDHandler/stuff/Stuff.h>


class Case : public CaseResource {

public:
	Case() : CaseResource(ResourceHandler::GetInstance(), "") {

	}
	Case(std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p) {

	}

	int getSceneCount() {
		return this->getScenesIdInCase()->size();
	}
	
};


class Scene : public SceneResource {
public:
	Scene(SceneType scnType) : SceneResource(ResourceHandler::GetInstance(), "", scnType, Ogre::Vector3(), Ogre::Vector4(), Ogre::Vector3()) {

	}
	Scene(SceneType scnType, std::string name_p) : SceneResource(ResourceHandler::GetInstance(), name_p, scnType, Ogre::Vector3(), Ogre::Vector4(), Ogre::Vector3()) {

	}
	Scene(SceneType scnType, std::string name_p,Ogre::Vector3 pos_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) : 
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, pos_p, orientation_p, scale_p) {

	}

};


// Manages Case. i.e all the Scenes in a single save file
// Also manages the integration of ResourceScenes and Ogre::Scenes
class CaseHandler {

private:

	StuffHandler* stuffHandler;
	Ogre::SceneManager* oScnManager;

	Case* currentCase;


protected:

	// Ogre::Scenes must remain consistant with ResourceHandler.
	void checkIntegrity();




public:

	// CASE FUNCTIONS

	CaseHandler(StuffHandler* stuffHandler_p, Ogre::SceneManager* oScnManager_p) {
		stuffHandler = stuffHandler_p;
		oScnManager = oScnManager_p;

		currentCase = new Case("Unnamed Case");

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


};



class CaseErrorHandler : public std::exception {

public:

	CaseErrorHandler(const char* what) : std::exception(what) {
	}

};
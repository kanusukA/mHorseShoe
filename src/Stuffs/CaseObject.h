#pragma once

#include <Stuffs/SceneObject.h>


class Case : public CaseResource {
private:
	GDBuilderContext* GDBuilderCxt;

	std::vector<std::shared_ptr<Scene>>* sceneVec = new std::vector<std::shared_ptr<Scene>>();

public:


	Case(GDBuilderContext* builderCxt_p) : CaseResource(ResourceHandler::GetInstance(), "") {
		GDBuilderCxt = builderCxt_p;
	}
	Case(GDBuilderContext* builderCxt_p, std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p) {
		GDBuilderCxt = builderCxt_p;
	}

	// CREATEING A NEW SCENE
	std::weak_ptr<Scene> attachNewScene(std::string sceneName, SceneType scnType, Ogre::SceneNode* parentNode_p) {
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName, scnType,parentNode_p);
		std::shared_ptr<Scene> sScene(newScene,SceneDeleter);
		sceneVec->push_back(std::move(sScene));
		return sceneVec->at(sceneVec->size() - 1);
	}
	std::weak_ptr<Scene> attachNewSceneToRoot(std::string sceneName, SceneType scnType) {
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName, scnType);
		std::shared_ptr<Scene> sScene(newScene,SceneDeleter);
		sceneVec->push_back(std::move(sScene));
		return sceneVec->at(sceneVec->size() - 1);
	}

	std::weak_ptr<Scene> getwScene(int index) {
		return sceneVec->at(index);
	}

	void removeSceneByIndex(int index) {
		sceneVec->erase(sceneVec->begin() + index);
	}

	void removeAllScenes() {
		sceneVec->clear();
	}

	const std::vector<std::shared_ptr<Scene>>* getScenes() {
		return sceneVec;
	}

};

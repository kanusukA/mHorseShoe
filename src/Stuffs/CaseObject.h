#pragma once

#include <Stuffs/SceneObject.h>


class Case : public CaseResource {
private:
	GDBuilderContext* GDBuilderCxt;

	std::vector<std::shared_ptr<Scene>>* sceneVec = new std::vector<std::shared_ptr<Scene>>();

public:


	Case(GDBuilderContext* builderCxt_p) : CaseResource(ResourceHandler::GetInstance(), "","") {
		GDBuilderCxt = builderCxt_p;
	}
	Case(GDBuilderContext* builderCxt_p,std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p, "") {
		GDBuilderCxt = builderCxt_p;
	}
	Case(GDBuilderContext* builderCxt_p, std::string name_p, std::string filename_p) : CaseResource(ResourceHandler::GetInstance(), name_p, filename_p) {
		GDBuilderCxt = builderCxt_p;
	}

	// CREATEING A NEW SCENE
	std::weak_ptr<Scene> attachNewScene(std::string sceneName, SceneType scnType, Ogre::SceneNode* parentNode_p) { // add checks
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName, scnType,parentNode_p);
		if (newScene)
		{
			std::shared_ptr<Scene> sScene(newScene);
			sceneVec->push_back(std::move(sScene));
			return sceneVec->at(sceneVec->size() - 1);
		}
		return {}; // PASSES AN EMPTY WEAK POINTER IF THE SCENE CREATION FAILED
		
	}

	std::weak_ptr<Scene> attachNewSceneToRoot(std::string sceneName, SceneType scnType) { // add checks
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName, scnType);
		if (newScene)
		{
			std::shared_ptr<Scene> sScene(newScene);
			sceneVec->push_back(std::move(sScene));
			return sceneVec->at(sceneVec->size() - 1);
		}
		return {};
	}

	std::weak_ptr<Scene> getwScene(int index) {
		return sceneVec->at(index);
	}

	void removeSceneByIndex(int index) {
		sceneVec->erase(sceneVec->begin() + index);
	}

	const std::vector<std::shared_ptr<Scene>>* getScenes() {
		return sceneVec;
	}

	~Case() {
		ToastComponent::GetInstance()->addMessage("Case : " + name + " is being destroyed!");
		destoryCase();
	}

	void destoryCase() {

		sceneVec->clear();
	}

};

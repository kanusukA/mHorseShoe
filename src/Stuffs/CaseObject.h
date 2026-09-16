#pragma once

#include <Stuffs/SceneObject.h>


class Case : public CaseResource {
private:
	GDBuilderContext* GDBuilderCxt;

	std::vector<std::shared_ptr<Scene>> sceneVec{};

public:


	Case(GDBuilderContext* builderCxt_p, std::string name_p, std::string filename_p) : CaseResource(ResourceHandler::GetInstance(), name_p, filename_p) {
		GDBuilderCxt = builderCxt_p;
	}

	// CREATEING A NEW SCENE
	void attachNewScene(std::shared_ptr<Scene> scene_p ) { // add checks
		sceneVec.push_back(scene_p);
		
	}


	void removeSceneByIndex(int index) {
		sceneVec.erase(sceneVec.begin() + index);
	}

	const std::vector<std::shared_ptr<Scene>>* getScenes() {
		return &sceneVec;
	}

	~Case() {
		ToastComponent::GetInstance()->addMessage("Case : " + name + " is being destroyed!");
		destoryCase();
	}

	void destoryCase() {

		sceneVec.clear();
	}

};

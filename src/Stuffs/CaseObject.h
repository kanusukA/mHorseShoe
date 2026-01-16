#pragma once

#include <Stuffs/SceneObject.h>


class Case : public CaseResource {
private:
	GDBuilderContext* GDBuilderCxt;

	std::vector<std::shared_ptr<Scene>>* caseVec = new std::vector<std::shared_ptr<Scene>>();

public:


	Case(GDBuilderContext* builderCxt_p) : CaseResource(ResourceHandler::GetInstance(), "") {
		GDBuilderCxt = builderCxt_p;
	}
	Case(GDBuilderContext* builderCxt_p, std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p) {
		GDBuilderCxt = builderCxt_p;
	}

	// CREATEING A NEW SCENE
	void attachNewScene(std::string sceneName, SceneType scnType) {
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName, scnType);
		std::shared_ptr<Scene> sScene(newScene);
		caseVec->push_back(std::move(sScene));
	}

	std::weak_ptr<Scene> getwScene(int index) {
		return caseVec->at(index);
	}

	void removeSceneByIndex(int index) {
		caseVec->erase(caseVec->begin() + index);
	}

};

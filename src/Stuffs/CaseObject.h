#pragma once

#include <Stuffs/SceneObject.h>


class Case : public CaseResource {
private:
	std::vector<Scene*>* caseScenes = new std::vector<Scene*>();

public:
	Case(GDBuilderContext* builder_p) : CaseResource(ResourceHandler::GetInstance(),this, "") {

	}
	Case(GDBuilderContext* builder_p, std::string name_p) : CaseResource(ResourceHandler::GetInstance(),this, name_p) {

	}

	void addSceneToCase(Scene* scene_p){
		CaseResource::_addSceneToCase(scene_p->getId());
		caseScenes->push_back(scene_p);

	}

	void removeScene(Scene* scene_p) {
		CaseResource::_removeSceneByID(scene_p->getId());
		for (int i = 0; i < caseScenes->size(); i++) // TODO OPTIMIZATION REMOVE RUNS TWICES FOR ID AND SCENE OBJ
		{
			if (scene_p->getId() == caseScenes->at(i)->getId())
			{
				caseScenes->erase(caseScenes->begin() + i);
				break;
			}
		}
	}

	void removeByIndex(int index) {
		CaseResource::_removeSceneByIndex(index);
		caseScenes->erase(caseScenes->begin() + index);
	}


	std::vector<Scene*>* getScenes() { return caseScenes; }

};

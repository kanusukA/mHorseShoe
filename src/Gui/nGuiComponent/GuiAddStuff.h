#pragma once

#include <base/Mediator.h>


class GuiAddStuffModel : public ModelComponent {

public:
	CaseHandler* caseHandler;

	std::shared_ptr<Scene> selectedScene;
	std::shared_ptr<Case> selectedCase;

	bool showAddCase = false;
	bool showAddSceneToCase = false;
	bool showAddSceneToScene = false;

	GuiAddStuffModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {
		caseHandler = this->gdSource->getCaseHandler();
	}

	void addCase(std::string caseName, std::string fileName) {
		if (caseName.empty())
		{
			ToastComponent::GetInstance()->addMessage("Case Name Empty!");
			return;
		}
		caseHandler->createCase(caseName, fileName);
		showAddCase = false;
	}

	void addScene(std::shared_ptr<Case> pCase,std::string sceneName) {
		if (sceneName.empty())
		{
			ToastComponent::GetInstance()->addMessage("Scene Name Empty!");
			return;
		}
		caseHandler->createScene(sceneName);
		pCase->attachNewScene(caseHandler->mScenes->back());
		showAddSceneToCase = false;
		
	}

	void addSceneToScene(std::shared_ptr<Scene> pScene ,std::string sceneName) {
		if (sceneName.empty())
		{
			ToastComponent::GetInstance()->addMessage("Scene Name Empty!");
			return;
		}
		caseHandler->createScene(sceneName);
		pScene->attachNewScene(caseHandler->mScenes->back());
		showAddSceneToScene = false;
		
	}

};

class GuiAddOverView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	GuiAddOverView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;

};


class GuiAddCaseView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	std::string caseName = "";
	std::string caseFileName = "";

	GuiAddCaseView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;


};

class GuiAddSceneView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	std::string sceneName = "";

	GuiAddSceneView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;


};
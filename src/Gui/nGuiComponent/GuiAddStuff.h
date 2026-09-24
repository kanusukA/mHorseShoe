#pragma once

#include <base/Mediator.h>


class GuiAddStuffModel : public ModelComponent {

public:
	CaseHandler* caseHandler;

	std::shared_ptr<Scene> selectedScene;
	std::shared_ptr<Case> selectedCase;
	std::shared_ptr<Object> selecetdObject;

	std::vector<std::filesystem::path>* renderMeshes = nullptr;
	std::vector<std::filesystem::path>* shaders = nullptr;

	bool showAddCase = false;
	bool showAddSceneToCase = false;
	bool showAddSceneToScene = false;
	bool showAddObject = false;

	GuiAddStuffModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {
		caseHandler = this->gdSource->getCaseHandler();
		renderMeshes = this->gdSource->getResourceHandler()->fetchResourceGroupVecByIndex(ResourceGroup::RENDER_MESH_PATH);
		shaders = this->gdSource->getResourceHandler()->fetchResourceGroupVecByIndex(ResourceGroup::SHADER);
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

	void addObject(std::string name_p, std::shared_ptr<Scene> scn, std::filesystem::path* renderMesh, std::filesystem::path* vertPath, std::filesystem::path* fragPath) {
		caseHandler->createObject(name_p, renderMesh, vertPath, fragPath);
		scn->attachNewObject(caseHandler->mObjects->back());

		showAddObject = false;
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

class GuiAddObjectView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	std::string objectName = "";

	int selectedRenderMesh = 0;
	int selectedVertShader = 0;
	int selectedFragShader = 0;

	GuiAddObjectView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;

};
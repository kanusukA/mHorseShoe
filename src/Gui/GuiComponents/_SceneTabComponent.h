#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();
	bool rootSceneNodeSelected = true;
public:

	std::weak_ptr<std::string> defaultCase;

	std::weak_ptr<Scene> selectedScene;
	std::weak_ptr<Object> selectedObject;

	std::string* inputCaseName = new std::string("");
	int selectedCase = 0;

	int selectedSavedCaseFile = 0;

	std::string* inputSceneName = new std::string("");
	SceneType scnType = SceneType::STATIC;

	// MESHS FROM RESOURCE HANDLER
	std::string* inputObjectname = new std::string("");
	PhysXType physxType = PhysXType::Static;

	int selectedMesh = 0;

	SceneTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		selectedScene = ModelComponent::selectedScene->selScene;
	}

	void saveCase() {
		gdSource->getCaseHandler()->saveAll();
	}

	void loadCase() {
		// TODO
		gdSource->getCaseHandler()->loadCase(ModelComponent::savedCaseFiles->at(selectedSavedCaseFile));
	}

	void addCase() {
		this->gdSource->getCaseHandler()->CreateCase(*inputCaseName);
	}

	void addScene() {
		if (rootSceneNodeSelected)
		{
			CaseHandler::selectedCase.get()->attachNewSceneToRoot(*inputSceneName, scnType);
			
		}
		else {
			CaseHandler::selectedCase.get()->getwScene(selectedCase).lock()->attachNewScene(*inputSceneName, scnType);
			
		}
		
		
	}

	void addObject() {
		ModelComponent::selectedScene->selScene.lock()->attachNewObject(*inputObjectname, ModelComponent::meshDpVec->at(selectedMesh), physxType);
	}

	void deleteObject(int index) {
		ModelComponent::selectedScene->selScene.lock()->removeObjectByIndex(index);
	}

	void selectObject(std::weak_ptr<Object> wObject_p) {
		ModelComponent::selectObject(wObject_p);
	}

	void selectScene(std::weak_ptr<Scene> wScene_p) {
		rootSceneNodeSelected = false;
		ModelComponent::selectScene(wScene_p);
	}

	void selectCase(int index) {
		/*ModelComponent::selectCase(index);*/
	}

	void deleteScene(int index) {
		rootSceneNodeSelected = true;
		if (CaseHandler::selectedCase)
		{
			CaseHandler::selectedCase.get()->removeSceneByIndex(index);
		}
		/*ModelComponent::selectedCase->selCase.lock()->removeSceneByIndex(index);*/
	}

	void selectRootSceneNode() {
		rootSceneNodeSelected = true;
	}

	bool isRootScnNodeSelected() {
		return this->rootSceneNodeSelected;
	}

	void saveDefaultCase() {
		if (CaseHandler::selectedCase)
		{
			if (CaseHandler::selectedCase->getFileName())
			{
				gdSource->getCaseHandler()->saveDefaultCase(*CaseHandler::selectedCase->getFileName());
			}
			else {
				ToastComponent::GetInstance()->addMessage("The Selected Case Has no saved file. Save the file first and restart the program.");
			}
			
		}
		else {
			ToastComponent::GetInstance()->addMessage("Select a Case First!");
		}
		
		
	}

	// Button functions
	void refresh() {
		ResourceHandler::GetInstance()->loadSavedCases();
	};
	//void loadCase();

	void update(GUIUpdateEvent event) override {
		if (CaseHandler::selectedCase)
		{
			inputCaseName->clear();
			inputCaseName->append(CaseHandler::selectedCase->getName());
		}
		if (!ModelComponent::selectedScene->selScene.expired())
		{
			selectedScene = ModelComponent::selectedScene->selScene;
			inputSceneName->clear();
			inputSceneName->append(selectedScene.lock()->getName());
		}
		if (!ModelComponent::selectedObject->selObject.expired())
		{
			selectedObject = ModelComponent::selectedObject->selObject;
			inputObjectname->clear();
			inputObjectname->append(ModelComponent::selectedObject->selObject.lock()->getMeshName());
		}
	}
	

};

class SceneTabComponent : public ViewComponent {

private:
	SceneTabComponent();

	// MODELS
	SceneTabModelComponent* model;


public:


	SceneTabComponent(const char* name_p,
		SceneTabModelComponent* sceneTabComponent
		) : ViewComponent(name_p){
		model = sceneTabComponent;
	};

	void view() override;

};


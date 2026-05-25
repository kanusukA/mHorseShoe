#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();
	bool rootSceneNodeSelected = true;
public:

	std::weak_ptr<std::string> defaultCase;

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
			ModelComponent::selectedCase->selCase.lock()->attachNewSceneToRoot(*inputSceneName, scnType);
		}
		else {
			ModelComponent::selectedScene->selScene.lock()->attachNewScene(*inputSceneName, scnType);
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
		ModelComponent::selectCase(index);
	}

	void deleteScene(int index) {
		rootSceneNodeSelected = true;
		ModelComponent::selectedCase->selCase.lock()->removeSceneByIndex(index);
	}

	void selectRootSceneNode() {
		rootSceneNodeSelected = true;
	}

	bool isRootScnNodeSelected() {
		return this->rootSceneNodeSelected;
	}

	void saveDefaultCase() {
		if (!defaultCase.expired())
		{
			*defaultCase.lock().get() = *inputCaseName;
		}

		gdSource->getCaseHandler()->saveDefaultCase();
		
	}

	// Button functions
	void refresh();
	//void loadCase();

	virtual void update() override {
		if (!ModelComponent::selectedCase->selCase.expired())
		{
			inputCaseName->clear();
			inputCaseName->append(ModelComponent::selectedCase->selCase.lock()->getName());
		}
		if (!ModelComponent::selectedScene->selScene.expired())
		{
			inputSceneName->clear();
			inputSceneName->append(ModelComponent::selectedScene->selScene.lock()->getName());
		}
		if (!ModelComponent::selectedObject->selObject.expired())
		{
			inputObjectname->clear();
			inputObjectname->append(ModelComponent::selectedObject->selObject.lock()->getMeshName());
		}
	}
	

};

class SceneTabComponent : public ViewComponent {

private:
	SceneTabComponent();

	// MODELS
	SceneTabModelComponent* scnTabModel;


public:


	SceneTabComponent(const char* name_p,
		SceneTabModelComponent* sceneTabComponent
		) : ViewComponent(name_p){
		scnTabModel = sceneTabComponent;
	};

	void view() override;

};


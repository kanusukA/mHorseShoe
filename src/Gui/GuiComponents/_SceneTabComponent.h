#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();
	bool rootSceneNodeSelected = true;
public:

	std::string* inputCaseName = new std::string("");
	int selectedCase = 0;


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

	// Button functions
	void refresh();
	void loadCase();
	void saveCase();

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


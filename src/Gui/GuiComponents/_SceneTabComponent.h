#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();
	bool rootSceneNodeSelected = true;
public:

	// ID MANAGING (IGNORE)
	int* id = new int(0);
	
	std::vector<Case*>* cases;

	Case* currentCase;

	std::string* inputSceneName = new std::string("");
	SceneType scnType = SceneType::STATIC;

	// MESHS FROM RESOURCE HANDLER
	std::string* inputObjectname = new std::string("");
	std::vector<std::filesystem::path>* meshes;
	int selectedMesh = 0;

	SceneTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		cases = this->gdSource->getCaseHandler()->getAllCases();
		currentCase = this->gdSource->getCaseHandler()->getCurrentCase();
		meshes = this->gdSource->getResourceHandler()->getRenderMeshLoaded();
		
	}


	std::string getCaseName() { return currentCase->getName(); }
	
	SceneResource* getCaseScene(ResID scnID) { return this->gdSource->getResourceHandler()->fetchSceneResourceByID(scnID); }

	void addScene() {
		if (rootSceneNodeSelected)
		{
			this->gdSource->getCaseHandler()->CreateSceneAttachToCase(scnType, *inputSceneName);
		}
		else {
			Scene* new_scn = this->gdSource->getCaseHandler()->CreateScene(scnType, *inputSceneName);
			if (new_scn)
			{
				currentCase->getSelectedScene()->addScene(new_scn);
			}
			
		}
		
	}

	void selectScene(Scene* scn_p) {
		currentCase->selectScene(scn_p);
		rootSceneNodeSelected = false;
	}
	void deleteScene(Scene* scn_p) {
		currentCase->removeScene(scn_p);
		rootSceneNodeSelected = true;
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


#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();

public:
	
	std::vector<Case*>* cases;

	Case* currentCase;

	std::string* inputSceneName = new std::string("");
	SceneType scnType = SceneType::STATIC;

	SceneTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		cases = this->gdSource->getCaseHandler()->getAllCases();
		currentCase = this->gdSource->getCaseHandler()->getCurrentCase();
		
	}


	std::string getCaseName() { return currentCase->getName(); }
	
	SceneResource* getCaseScene(ResID scnID) { return this->gdSource->getResourceHandler()->fetchSceneResourceByID(scnID); }

	void addScene() {
		this->gdSource->getCaseHandler()->CreateScene(scnType,*inputSceneName);
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


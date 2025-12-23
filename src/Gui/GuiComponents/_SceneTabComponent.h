#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {

private:
	SceneTabModelComponent();

public:
	
	std::vector<CaseResource*>* cases = nullptr;

	Case* currentCase;

	SceneTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		cases = this->gdSource->getResourceHandler()->getAllCase();
		currentCase = this->gdSource->getCaseHandler()->getCurrentCase();

	}

	std::string getCaseName() { return currentCase->getName(); }
	
	SceneResource* getCaseScene(ResID scnID) { return this->gdSource->getResourceHandler()->fetchSceneResourceByID(scnID); }



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


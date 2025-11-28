#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabModelComponent : public ModelComponent {
private:
	SceneTabModelComponent();

public:
	
	std::vector<std::string>* cases = nullptr;

	SceneTabModelComponent(const char* name_p) : ModelComponent() {
		this->name = name_p;
		
	}

	void init() override {
		cases = this->gdSource->getSceneHandler()->getCases();
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
		) : ViewComponent(){
		this->name = name_p;
		scnTabModel = sceneTabComponent;
	};

	void view() override;

};


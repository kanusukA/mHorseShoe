#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class ScenePanelTabModelComponent : public ModelComponent
{
private:

	

public:

	std::vector<SceneResource*>* scenes;

	ScenePanelTabModelComponent(const char* name_p) : ModelComponent(name_p)
	{

	}

	void init() override {
		scenes = this->gdSource->getResourceHandler()->getAllScenes();
	}
};

class ScenePanelTabComponent : public ViewComponent
{
private:

	ScenePanelTabModelComponent* sceneModel;

public:
	ScenePanelTabComponent(const char* name_p,ScenePanelTabModelComponent* sceneModel_p) : ViewComponent(name_p) {
		sceneModel = sceneModel_p;
	}

	void view() override;
};


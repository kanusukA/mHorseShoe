#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class ScenePanelTabModelComponent : public ModelComponent
{


public:

	std::vector<Ogre::SceneNode*>* DynamicScenes;
	std::vector<Ogre::SceneNode*>* StaticScenes;
	std::vector<Ogre::SceneNode*>* MeshScenes;

	ScenePanelTabModelComponent(const char* name_p) : ModelComponent(name_p)
	{

	}

	void init() override {
		DynamicScenes = this->gdSource->getSceneHandler()->getDynamicScenes();
		StaticScenes = this->gdSource->getSceneHandler()->getStaticScenes();
		MeshScenes = this->gdSource->getSceneHandler()->getMeshScenes();
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


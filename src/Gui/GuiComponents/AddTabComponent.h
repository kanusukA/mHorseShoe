#pragma once



#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>
#include <imgui_stdlib.h>


class AddTabModelComponent : public ModelComponent
{
	Ogre::Quaternion* normalizer = new Ogre::Quaternion();


public:

	std::string* objectName = new std::string("");

	Ogre::StringVectorPtr RenderMeshes;
	std::vector<std::filesystem::path>* ColliderMeshes;

	int pos[3] = { 0,0,0 };
	float rot[4] = { 0.0,0.0,0.0,0.0 };
	float colSize[3] = { 1,1,1 };

	float* mass = new float(1.0);


	int selectedScenePos = 0;
	int renderMeshesPos = 0;
	int colliderMeshesPos = 0;

	StuffType SelectedStuffType = STUFF_STATIC;

	std::string* CreateSceneNode = new std::string(""); // PORT TO IMGUI_STD_STRING

	AddTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		// TODO ADD SCENES IN ADD TAB
		/*MeshScenes = this->gdSource->getCaseHandler()->getMeshScenes();
		StaticScenes = this->gdSource->getCaseHandler()->getStaticScenes();
		DynamicScenes = this->gdSource->getCaseHandler()->getDynamicScenes();
		MeshScenes = this->gdSource->getCaseHandler()->getMeshScenes();*/

		RenderMeshes = this->gdSource->getResourceHandler()->ogreRenderMeshes;
		ColliderMeshes = this->gdSource->getResourceHandler()->colliderMeshes;

		
	}

	void normalizeRot() {
		normalizer->w = rot[0];
		normalizer->x = rot[1];
		normalizer->y = rot[2];
		normalizer->z = rot[3];

		normalizer->normalise();

		if (normalizer->isNaN())
		{
			rot[0] = 0;
			rot[1] = 0;
			rot[2] = 0;
			rot[3] = 0;

			normalizer->w = rot[0];
			normalizer->x = rot[1];
			normalizer->y = rot[2];
			normalizer->z = rot[3];
			
		}
		else {
			rot[0] = normalizer->w;
			rot[1] = normalizer->x;
			rot[2] = normalizer->y;
			rot[3] = normalizer->z;
		}

		

		
	}
	

	void createSceneBtn() {
		switch (SelectedStuffType)
		{
		case STUFF_DYNAMIC:
			gdSource->getCaseHandler()->CreateScene(SceneType::DYNAMIC, *CreateSceneNode);
			break;
		case STUFF_STATIC:
			gdSource->getCaseHandler()->CreateScene(SceneType::STATIC, *CreateSceneNode);
			break;
		case STUFF_MESH_ONLY:
			gdSource->getCaseHandler()->CreateScene(SceneType::MESH, *CreateSceneNode);
			break;
		default:
			break;
		}
	}

	void add() {
		// TODO add stuffHandler Object creation
	}


};

class AddTabComponent : public ViewComponent
{
private:
	AddTabModelComponent* addModel;

public:
	AddTabComponent(const char* name_p, AddTabModelComponent* addModel_p) : 
		ViewComponent(name_p) {
		addModel = addModel_p;
	}

	void view() override;

};


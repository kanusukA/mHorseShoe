#pragma once



#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>



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

	

	std::string* CreateSceneNode = new std::string(""); // PORT TO IMGUI_STD_STRING

	AddTabModelComponent(const char* name_p) : ModelComponent(name_p) {
		
	}

	void init() override {
		

		
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
	}

	void add() {
		
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


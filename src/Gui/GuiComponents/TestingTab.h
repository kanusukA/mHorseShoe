#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>
#include <imgui_stdlib.h>

class TestingTabModelComponent : public ModelComponent {

public:

	std::vector<ResID>* masterList;
	std::vector<CaseResource>* caseResources;
	std::vector<ResID>* scnInCaseResource;

	std::vector<SceneResource>* sceneResources;

	std::string* caseName = new std::string("");


	// SCENE
	ResID selectedScnID;

	std::string scnName = " ";

	float scn_pos[3] = { 0,0,0 };

	float scn_orientation[4] = { 0,0,0,0 };

	float scn_scale[3] = { 0,0,0 };

	SceneType scnType;

	int selectedCaseInSceneTab = 0;


	TestingTabModelComponent(const char* name_p) : ModelComponent(name_p){

	}
	// Scene are not being added to Case
	void init() override {
		masterList = this->gdSource->getResourceHandler()->getMasterList();
		caseResources = this->gdSource->getResourceHandler()->getAllCase();
		sceneResources = this->gdSource->getResourceHandler()->getAllScenes();
	}
	
	// CASE
	void addCase() {

		Case myCase = Case(caseName->c_str(), this->gdSource->getResourceHandler());
		
	}

	void AddSceneToCase(ResID caseID) {
		CaseResource* caseRes = this->gdSource->getResourceHandler()->fetchCaseResourceByID(caseID);
		caseRes->addSceneToCase(selectedScnID);
	}

	void getScenesInCase(ResID caseID) {
		CaseResource* caseRes = this->gdSource->getResourceHandler()->fetchCaseResourceByID(caseID);

		scnInCaseResource = caseRes->getScenesInCase();
	}

	// SCENE
	void getSceneDetails(ResID id) {
		SceneResource* scnRes = this->gdSource->getResourceHandler()->fetchSceneResourceByID(id);

		Scene* scnCast = static_cast<Scene*>(scnRes);

		scnName = scnCast->getName();

		this->guiFramework->setOgreVec3(scn_pos,scnCast->getScnPosition());
		this->guiFramework->setOgreVec3(scn_scale, scnCast->getScnScale());
		this->guiFramework->setOgreVec4(scn_orientation, scnCast->getScnOrientation());
		
		scnType = SceneType(scnCast->getScnType());

		selectedScnID = id;

	}

	void addScene() {
		
		Scene myScene = Scene(this->gdSource->getResourceHandler(), scnName, scnType,
			Ogre::Vector3(scn_pos[0], scn_pos[1], scn_pos[2]),
			Ogre::Vector3(scn_scale[0], scn_scale[1], scn_scale[2]),
			Ogre::Vector4(scn_orientation[0], scn_orientation[1], scn_orientation[2], scn_orientation[3])
			);
	}

};

class TestingTabComponent : public ViewComponent
{
private:
	TestingTabModelComponent* testModel;

public:
	TestingTabComponent(const char* name_p,TestingTabModelComponent* testModel_p) : ViewComponent(name_p) {
		testModel = testModel_p;
	}

	void view() override;
};


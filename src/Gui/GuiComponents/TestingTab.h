#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>
#include <imgui_stdlib.h>

class TestingTabModelComponent : public ModelComponent {

public:

	std::vector<ColliderMeshResource*>* colliderResource;
	std::vector<RenderMeshResource*>* renderResource;

	std::vector<ImageResource*>* imageResource;

	std::string* renderName = new std::string();

	std::string* colliderName = new std::string();

	std::vector<ResID>* masterList;
	std::vector<CaseResource*>* caseResources;
	std::vector<ResID>* scnInCaseResource;

	std::vector<SceneResource*>* sceneResources;

	std::string* caseName = new std::string("");

	std::vector<ObjectResource*>* objectResources;
	std::string* objName = new std::string("");
	PhysXType physXType = PhysXType::Static;
	float mass = 0;

	std::vector<ShaderResource*>* shaderResources;
	std::string* shaderName = new std::string("");
	std::string* shaderFileName = new std::string("");
	ShaderType shaderType = ShaderType::Fragment;

	std::vector<ShaderVar>* shaderVars = new std::vector<ShaderVar>();

	ShaderVar* shadervar = new ShaderVar();

	std::vector<MaterialResource*>* materialResources;

	// SCENE
	ResID selectedScnID;

	std::string scnName = "";

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
		objectResources = this->gdSource->getResourceHandler()->getAllObjects();
		shaderResources = this->gdSource->getResourceHandler()->getAllShader();
		materialResources = this->gdSource->getResourceHandler()->getAllMaterial();
		colliderResource = this->gdSource->getResourceHandler()->getAllColliderMesh();
		renderResource = this->gdSource->getResourceHandler()->getAllRenderMesh();
		imageResource = this->gdSource->getResourceHandler()->getAllImages();
	}
	
	// CASE
	void addCase() {

		Case* myCase = new Case(*caseName, this->gdSource->getResourceHandler());
		
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
		
		scnType = SceneType(scnCast->getSceneType());

		selectedScnID = id;

	}

	void addScene() {
		
		Scene* myScene = new Scene(this->gdSource->getResourceHandler(), scnName, scnType,
			Ogre::Vector3(scn_pos[0], scn_pos[1], scn_pos[2]),
			Ogre::Vector3(scn_scale[0], scn_scale[1], scn_scale[2]),
			Ogre::Vector4(scn_orientation[0], scn_orientation[1], scn_orientation[2], scn_orientation[3])
			);

		
	}

	// Shader

	void getShader(ResID id) {
		ShaderResource* shader = this->gdSource->getResourceHandler()->fetchShaderResourceByID(id);

		*shaderName = shader->getName();
		*shaderFileName = shader->getShaderFileName();

		shaderType = shader->getShaderType();

		shaderVars = shader->getShaderVars();

	}


	void addShader() {
		ShaderResource* shader = new ShaderResource(this->gdSource->getResourceHandler(), *shaderName, shaderType, *shaderFileName);

	}

	void addCollider() {
		ColliderMeshResource* colRes = new ColliderMeshResource(this->gdSource->getResourceHandler(), *colliderName, "test", "test");

	}

	void addRender() {
		RenderMeshResource* renderMesh = new RenderMeshResource(this->gdSource->getResourceHandler(), *renderName, "test", "test");

	}

	void addObject() {
		ObjectResource* obj = new ObjectResource(this->gdSource->getResourceHandler(), *this->objName, physXType);
	}

	void addImage() {
		ImageResource* image = new ImageResource(this->gdSource->getResourceHandler(), "C://Deez");
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


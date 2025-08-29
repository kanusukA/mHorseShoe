#pragma once

#ifndef GUIMEDIATOR_H
#define GUIMEDIATOR_H


#include<Gui/GuiTabs.h>
#include <base/Mediator.h>

#include <OgreImGuiOverlay.h>
#include <GDHandler/stuff/Stuff.h>



struct GuiParam {
	// VISUAL PARAM
	bool showRenderShapes = true;
	bool showDebugLines = false;

	// STATUS TAB
	bool showStatus = false;
	ImVec2 statusPosition;

	// ADD TAB
	bool showAdd = false;

};

struct Diegnostics {

	int* fps = new int(0);

	float* TotalFrameTime = new float(0.0);
	float* renderFrameTime = new float(0.0);

};

struct AddParam {

	int positionRenderMesh = 0;
	int positionColliderMesh = 0;

	// Strings are set in a pointer because IMGUI takes string pointers for Input Text. It's more simpler
	std::string* obj_name = new std::string("Object Name");
	std::string* msh_name = new std::string("Mesh File Name");
	std::string* col_name = new std::string("Collider name");

	// type : 
	// 0 - DYNAMIC
	// 1 - STATIC
	// 2 - MESH
	StuffType stuffType = StuffType::STUFF_DYNAMIC;
	int* mass = new int(1);

	int pos[3] = { 0,0,0 };
	int rot[3] = { 0,0,0 };
	int c_size[3] = { 0,0,0 };

};

struct GuiObjectTab {
	std::vector<StuffDynamic*>* stuffDyn;
	std::vector<StuffMesh*>* stuffMesh;
	std::vector<StuffStatic*>* stuffStatic;
};

// structure class used to handle which tab is being displayed 
// currently
struct GuiTabsVisibility {
	bool statusTab = false;
	bool addObjectTab = false;
	bool resourcesTab = false;
	bool lightTab = false;
	bool allTabs = true;
};

struct LightTab {

	const char* lightTypeNames[4] = { "Point","Direction","Spot","Rect"};
	
	std::string* mdrlName = new std::string("main Light");
	std::string* lightName = new std::string("light");
	int lightTypePos = 0;
	int mDirection[3] = { 0,0,0 };
	int lightPos[3] = { 0,0,0 };
	int lightDir[3] = { 0,0,0 };
	int lightPowerScale = 1;
	int powerScale = 1;

};

struct HeightMapTab {

	int imgPos = 0;
	int grassImgPos = 0;
	int	width = 0;
	int	height = 0;
	float* displacementFac = new float(1.0f);
	int* vertSize = new int(1);

	int* grassDensity = new int(0);
	float* grassScale = new float(0.0f);

};

struct TerrainTab {
	int terrainSize = 0;
	int blocks = 0;
	int selectedMaterial = 0;
	int selectedHeightMap = -1;
	float displacement = 0.0f;
	float scale = 1.0f;

};

struct RSUSParam
{
	int selectedMaterial = 0;
	RSUShader* rsusObj;
};

struct SceneTab {
	std::vector<Ogre::SceneNode*>* StaticScenes;
	std::vector<Ogre::SceneNode*>* DynamicScenes;
	std::vector<Ogre::SceneNode*>* MeshScenes;

	std::vector<Ogre::SceneNode*>* selectedScenes = StaticScenes;
	int selectedScenePos = 0;

	std::string CreateSceneNode = "";

};

class GuiComponent : public MediatorComponent {
private:

	Diegnostics* diegnos;

	AddParam* addParam = new AddParam();
	GuiParam* guiParam = new GuiParam();
	GuiTabsVisibility* guiTabs = new GuiTabsVisibility();

	RSUSParam* rsusParam = new RSUSParam();
	

	// selectableObj
	GuiSelectableObject* selectedObj;


public:

	GuiObjectTab* objParam = new GuiObjectTab();
	LightTab* lightTab = new LightTab();
	HeightMapTab* heightMapTab = new HeightMapTab();
	TerrainTab* terrainTab = new TerrainTab();

	SceneTab* sceneTab = new SceneTab();

	// Set Stuff Objects
	void setStuffObjects(std::vector<StuffDynamic*>* dyn, std::vector<StuffMesh*>* mesh, std::vector<StuffStatic*>* stat) {
		objParam->stuffDyn = dyn;
		objParam->stuffMesh = mesh;
		objParam->stuffStatic = stat;
	}

	void setSelectableObject(GuiSelectableObject* obj) {
		selectedObj = obj;
	}
	GuiSelectableObject* getSelectedObject() {
		return selectedObj;
	}

	void addObject();

	void showRenderMeshes();

	void deleteSelectedObj();

	void showDebugLines();

	void setHeightMap();

	// Add Mesh functions
	void setRenderMeshName(std::string name) {
		*addParam->msh_name = name;
	}
	void setColliderMeshName(std::string name) {
		*addParam->col_name = name;
	}

	void setObjectName(std::string name) {
		*addParam->obj_name = name;
	}

	std::string* getRenderMeshName() {
		return addParam->msh_name;
	}
	std::string* getColliderMeshName() {
		return addParam->col_name;
	}
	
	std::string* getObjectName() {
		return addParam->obj_name;
	}

	StuffType& getPhysicsType() {
		return this->addParam->stuffType;
	}
	void setPhysicsType(StuffType type) {
		this->addParam->stuffType = type;
	}

	int* getMass() {
		return this->addParam->mass;
	}

	int* getPosition() {
		return this->addParam->pos;
	}
	int* getRotation() {
		return this->addParam->rot;
	}
	int* getColliderSize() {
		return this->addParam->c_size;
	}

	void setUpMainDirectionalLight() {
		return this->_mediator->Notify(this, GDEVENT_ADD_MDRL);
	}

	void setUpLight() {
		return this->_mediator->Notify(this, GDEVENT_ADD_LIGHT);
	}

	// Gui Param functions
	void showGuiTab(bool visibility, GuiTabs tab);

	GuiTabsVisibility* getGuiTabVisibility() {
		return this->guiTabs;
	}

	void setDiegnostics(Diegnostics* die) {
		diegnos = die;
	}
	Diegnostics* getDiegnostics() {
		return diegnos;
	}

	// Terrain
	void createTerrain() {
		this->_mediator->Notify(this, GDEVENT_GENERATE_TERRAIN);
	}


	// RSUS
	RSUSParam* getRSUSParam() {
		return rsusParam;
	}

	void saveRsusObj();

	void getSelObjFrag() {
		this->_mediator->Notify(this, GDEVENT_GETSELECTEDOBJFRAGMENT);
		
		this->rsusParam->rsusObj = RSUS::GetInstance()->rsusObj;
		
	};

	void sendMaterialRsus() {
		RSUS::GetInstance()->readMaterial(ResourceHandler::GetInstance()->OgreMaterials->at(this->rsusParam->selectedMaterial));
		this->rsusParam->rsusObj = RSUS::GetInstance()->rsusObj;
	}
	void updateFragRsusInt(std::string varName, int val) {
		RSUS::GetInstance()->updateFragParameterInt(varName, val);
	}
	void updateFragRsusFloat(std::string varName, float* val) {
		RSUS::GetInstance()->updateFragParameterFloat(varName, val);
	}
	void updateFragRsusFloat2(std::string varName, float* val) {
		RSUS::GetInstance()->updateFragParameterFloat2(varName, Ogre::Vector2(val[0],val[1]));
	}
	void updateFragRsusFloat3(std::string varName, float* val) {
		RSUS::GetInstance()->updateFragParameterFloat3(varName, Ogre::Vector3(val[0],val[1],val[2]));
	}
	void updateFragRsusFloat4(std::string varName, Ogre::Vector4 val) {
		RSUS::GetInstance()->updateFragParameterFloat4(varName, val);
	}
	void updateVertRsusInt(std::string varName, int val) {
		RSUS::GetInstance()->updateVertParameterInt(varName, val);
	}
	void updateVertRsusFloat(std::string varName, float* val) {
		RSUS::GetInstance()->updateVertParameterFloat(varName, val);
	}
	void updateVertRsusFloat2(std::string varName, float* val) {
		RSUS::GetInstance()->updateVertParameterFloat2(varName, Ogre::Vector2(val[0],val[1]));
	}
	void updateVertRsusFloat3(std::string varName, float* val) {
		RSUS::GetInstance()->updateVertParameterFloat3(varName, Ogre::Vector3(val[0],val[1],val[2]));
	}
	void updateVertRsusFloat4(std::string varName, Ogre::Vector4 val) {
		RSUS::GetInstance()->updateVertParameterFloat4(varName, val);
	}
	
};

#endif // !GUIMEDIATOR_H









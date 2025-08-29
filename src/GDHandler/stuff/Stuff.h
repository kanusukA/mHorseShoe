#pragma once

#ifndef STUFF_H
#define STUFF_H

#include <monster/Monster.h>
#include <kint/Kint.h>




using namespace physx;

enum StuffType {
	STUFF_DYNAMIC,
	STUFF_STATIC,
	STUFF_MESH_ONLY
};

PxVec3* mtkVectors(Ogre::Vector3* vec);

class Stuff
{

public:

	Ogre::SceneNode* scnNode;
	Ogre::String name;

	StuffType type;
	int indexId;

	void setSceneNode(Ogre::SceneNode* scnNode);
	Ogre::MovableObject* getEntity();
	void setName(Ogre::String nm);

	void showRenderer(bool value);

	void setCastShadow(bool value);


	virtual void setPxRigidDynamic(PxRigidDynamic* dynActor) {};
	virtual void setPxRigidStatic(PxRigidStatic* staActor) {};

	virtual void setPosition(Ogre::Vector3 position) {};
	virtual void setRotation(Ogre::Vector3 rotation) {};

	virtual void moveStuffBy(Ogre::Vector3 addPosition); // adds vector to the current position of the stuff

	virtual bool update(float deltaTime) { return true; };


};

class StuffStatic : public Stuff
{
private:

	std::string colliderMesh;

public:



	StuffStatic() {
		this->type = STUFF_STATIC;
	}

	PxRigidStatic* sActor;

	void setPxRigidStatic(PxRigidStatic* staActor) override;

	void setRotation(Ogre::Vector3 rot) override;
	void setPosition(Ogre::Vector3 pos) override;

	void moveStuffBy(Ogre::Vector3 addPos) override;

	// used by scene manager
	std::string _getColliderMesh() { return colliderMesh; }
	void _setColliderMesh(std::string colMesh) { colliderMesh = colMesh; }

};

// SET POSITION AND ROTATION FOR ALL
class StuffDynamic :public Stuff
{

private:

	std::string colliderMesh;
	int mass;

public:

	StuffDynamic() {
		this->type = STUFF_DYNAMIC;
	}

	PxRigidDynamic* sActor;

	void setPxRigidDynamic(PxRigidDynamic* dynActor) override;

	bool update(float deltaTime) override;

	void setRotation(Ogre::Vector3 rot) override;
	void setPosition(Ogre::Vector3 pos) override;

	void moveStuffBy(Ogre::Vector3 addPos) override; // move object from arrow keys!!!

	void getColliderData();

	// used by scene manager
	std::string _getColliderMesh() { return colliderMesh; }
	int _getMass() { return mass; }
	void _setColliderMesh(std::string colMesh) { colliderMesh = colMesh; }
	void _setMass(int m) { mass = m; }

};

class StuffMesh : public Stuff {

public:

	StuffMesh() {
		this->type = STUFF_MESH_ONLY;
	}

	void setRotation(Ogre::Vector3 rot) override;
	void setPosition(Ogre::Vector3 pos) override; 

	void moveStuffBy(Ogre::Vector3 addPos) override;

};

// Until functions
Ogre::Vector3 ktmPositionVec3(PxVec3 position);
Ogre::Quaternion ktmOrientation(PxQuat quat);

PxVec3 mtkPositionVec3(Ogre::Vector3 position);
PxQuat mtkOrientation(Ogre::Quaternion quat);

// test
struct LastObject
{
	std::string scnNodeName;
	std::string objName;
	std::string meshName;
	std::string colliderName;
	Ogre::Vector3 position;
	Ogre::Vector3 rotation;
	StuffType physicsType;
	float mass;
	Ogre::Vector3 colliderSize;

};

struct  GuiSelectableObject {
	std::string* selectingName = new std::string();
	Stuff* selectedStuff;
	StuffType type;
	bool* shadowCasting = new bool(true);
};

class StuffHandler {
private:

	// Kint
	bool showingOgreRendering = true;
	bool showingColliderDebug = false;
	Ogre::ManualObject* debugLines;
	Ogre::SceneNode* debugScnNode;
	void updateVisualDebug();

	// Stuff meshes
	std::vector<StuffMesh*>* stuffMeshes = new std::vector<StuffMesh*>();
	std::vector<StuffStatic*>* stuffStatics = new std::vector<StuffStatic*>();
	std::vector<StuffDynamic*>* stuffDynamics = new std::vector<StuffDynamic*>();

	// KEY STUFFS
	Terrain* terrainStuff;

	IKEYS* inputKeys = InputHandler::GetInstance()->getInputKeys();

	// last object
	LastObject* lastObject = new LastObject();

	// selectedstuff
	GuiSelectableObject* selectedObj = new GuiSelectableObject();

	Monster* monster = nullptr;
	Kint* kint = nullptr;

	void _addObject(
		std::string scnNodeName,
		std::string objName, 
		std::string meshName, 
		Ogre::Vector3 position, 
		Ogre::Vector3 rotation,
		bool castShadows
	);
	void _addObjectDynamic(
		std::string scnNodeName,
		std::string objName,
		std::string meshName,
		std::string colliderName,
		Ogre::Vector3 position,
		Ogre::Vector3 rotation,
		StuffType physicsType,
		float mass,
		Ogre::Vector3 colliderSize
	);
	void _addObjectStatic(
		std::string scnNodeName,
		std::string objName,
		std::string meshName,
		std::string colliderName,
		Ogre::Vector3 position,
		Ogre::Vector3 rotation,
		StuffType physicsType,
		Ogre::Vector3 colliderSize
	);

	

	void deleteObj(Stuff* stuff);

public:

	Monster* getMonsterRef() { return monster; }

	StuffHandler(Monster* ogre_monster, Kint* physx_kint) {
		monster = ogre_monster;
		kint = physx_kint;
		debugScnNode = monster->oScnManager->getRootSceneNode()->createChildSceneNode("Gizomo");
	}

	void addObject(
		std::string scnNodeName,
		std::string objName,
		std::string meshName,
		std::string colliderName,
		Ogre::Vector3 position,
		Ogre::Vector3 rotation,
		StuffType physicsType,
		float mass,
		Ogre::Vector3 colliderSize,
		bool duplicate = false,
		bool castShadows = true
	);

	void addGrass(Ogre::Real numOfGrass);

	std::vector<StuffDynamic*>* getStuffDynamicVec() {
		return this->stuffDynamics;
	}
	std::vector<StuffMesh*>* getStuffMeshVec() {
		return this->stuffMeshes;
	}
	std::vector<StuffStatic*>* getStuffStaticVec() {
		return this->stuffStatics;
	}

	void addLastObject();

	void setHeightMap(std::string loc, std::string grassMapLoc, float displacement, int vertSize, int grassDensity, float scale);

	// Opens fragment shader parameter of selectedobject
	void openFragmentShader();

	void addMDRL(std::string name, Ogre::Vector3& direction, int powerScale);
	void addLight(std::string name, Ogre::Vector3& position, Ogre::Vector3& direction, int powerScale, Ogre::Light::LightTypes type);

	// TERRAIN
	void createTerrain(Ogre::String materialStr, int terrainSize, int blocks);
	void createHeightmapTerrain(Ogre::String heightMap, Ogre::String materialStr, int terrainSize, int blocks, float displacement, float scale);
	

	// debug
	void showDebugPhysxMeshes();
	void showOgreRendering();

	void update(float deltaTime);
	void updateSelectedPos();

	GuiSelectableObject* getSelectedObj() { return this->selectedObj; }

	void deleteSelectedObj();

};


// SCENE HANDLER -------------------------------------------------------------------------------------------


enum SceneType {
	STATIC,
	DYNAMIC,
	MESH
};


class SceneHandler {
private:

	// Multi-Thread Shit
	static SceneHandler* pinstance_;
	static std::mutex mutex_;

	StuffHandler* stuffhandler;
	Ogre::SceneManager* oScnManager;

	std::vector<Ogre::SceneNode*> StaticScenes = std::vector<Ogre::SceneNode*>();
	std::vector<Ogre::SceneNode*> DynamicScenes = std::vector<Ogre::SceneNode*>();
	std::vector<Ogre::SceneNode*> MeshScenes = std::vector<Ogre::SceneNode*>();

	Ogre::Vector3 objPosToVecPos(std::string pos);
	Ogre::Vector4 objRotToVecRot(std::string orientation);

	void _travSceneNode(Ogre::SceneNode* node, int pos,std::vector<Ogre::SceneNode*>* sceNodes,int scnType);
	void _loadSceneNodes(Ogre::SceneNode* parNode, std::string scnNode, int scnType);
	// scnNode - The object is added to this node
	void _loadObject(SceneObject obj,std::string scnNode);

	void _saveObject(std::string scnName, SceneObject obj);
	void _saveScene(Ogre::SceneNode* scn);

protected:

	SceneHandler() {};
	~SceneHandler() {};

public:

	std::vector<Ogre::SceneNode*>* getStaticScenes() { return &StaticScenes; }
	std::vector<Ogre::SceneNode*>* getDynamicScenes() { return &DynamicScenes; }
	std::vector<Ogre::SceneNode*>* getMeshScenes() { return &MeshScenes; }


	// Creates 3 Main Scene Nodes
	void setStuffHandler(StuffHandler* stuff);
	
	void loadScenes();

	void saveScene(std::string scnName);

	void CreateScene(SceneType typ, std::string scnName);

	// Class should not be clonable
	SceneHandler(SceneHandler& copy) = delete;

	// Class should not be assignable
	void operator=(const SceneHandler&) = delete;

	static SceneHandler* GetInstance();



};


#endif // !STUFF_H


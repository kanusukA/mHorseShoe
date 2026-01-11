#pragma once


// Local
#include <feel/playerMovement.h>
#include <GDHandler/GDSystem.h>

// Third

// STL



class Scene;
class Case;
class Shader;
class Object;

class ResourceHandler;
class Monster;
class Kint;



// Primary Class.
// Used to build object with all required contexts and initializations
class GDBuilderContext : public GDSystem
{	
protected:

	ResourceHandler* resourceHandler;
	Monster* monster;
	Kint* kint;

	void startEngine();


public:

	GDBuilderContext(ResourceHandler* resourceHan_p, Monster* monster_p, Kint* kint_p) : GDSystem(monster_p) {
		resourceHandler = resourceHan_p;
		monster = monster_p;
		kint = kint_p;

		startEngine();
		
	}

	virtual void update(float deltaTime) {};

	PxRigidDynamic* getPxRigidDynamic(std::string name_p, PxTransform transform, PxGeometry* geometry, float mass);
	PxRigidStatic* getPxRigidStatic(std::string name_p, PxTransform transform, PxGeometry* geometry);

	// Check Functions
	bool sceneExists(std::string scnName_p);
	bool objectExists(std::string objName_p);
	
	// Create Functions
	virtual Ogre::SceneNode* createScene(std::string name_p) { return nullptr; }
	virtual Ogre::Entity* createObject(std::string name_p, Ogre::MeshPtr mesh_p) { return nullptr; }
	virtual Ogre::MeshPtr createMesh(std::string meshName_p) { return nullptr; }
	virtual Ogre::MaterialPtr createMaterial(std::string materialName_p) { return nullptr; }
	virtual void setShaderVars(Shader* shader) {};

	// Delete Functions
	void removeSceneNode(Ogre::SceneNode* scnNode_p);

	// CaseHandler Functions
	virtual ResID addMaterial(std::string matname_p) { return NULL; }
	virtual ResID addShader(Ogre::MaterialPtr mat_p, ShaderType type) { return NULL; }
	
	// SHADER FUNCTIONS

	virtual void selectShader(Shader* shader_p) {}


};

//class GDBuilderImpli {
//private:
//	GDBuilderContext* builderCxt;
//public:
//	GDBuilderImpli(GDBuilderContext* builderCxt_p) {
//		builderCxt = builderCxt_p;
//	}
//};



Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation);

Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat);
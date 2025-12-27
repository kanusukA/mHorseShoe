#pragma once


// Local
#include <GDHandler/stuff/Stuff.h>
#include <feel/Feel.h>
// Third

// STL



class Scene;
class Case;
class Shader;
class Object;

class ResourceHandler;
class Monster;
class Kint;
class Feel;



// Primary Class.
// Used to build object with all required contexts and initializations
class GDBuilderContext : public KeyHandler
{	
protected:

	ResourceHandler* resourceHandler;
	Monster* monster;
	Kint* kint;
	Feel* feel;

	void startEngine();


public:

	GDBuilderContext(ResourceHandler* resourceHan_p, Monster* monster_p, Kint* kint_p, Feel* feel_p) {
		resourceHandler = resourceHan_p;
		monster = monster_p;
		kint = kint_p;
		feel = feel_p;

		startEngine();
		
	}

	virtual void update(float deltaTime) {};

	PxRigidDynamic* getPxRigidDynamic(std::string name_p, PxTransform transform, PxGeometry* geometry, float mass);
	PxRigidStatic* getPxRigidStatic(std::string name_p, PxTransform transform, PxGeometry* geometry);
	
	// Create Functions
	virtual Ogre::SceneNode* createScene(std::string name_p) { return nullptr; }
	virtual Ogre::Entity* createObject(std::string name_p) { return nullptr; }
	virtual Ogre::Mesh* createMesh(std::string meshName_p) { return nullptr; }
	virtual Ogre::MaterialPtr createMaterial(std::string materialName_p) { return nullptr; }
	virtual void setShaderVars(Shader* shader) {};

	// CaseHandler Functions
	virtual ResID addMaterial(std::string matname_p) { return NULL; }
	virtual ResID addShader(Ogre::MaterialPtr mat_p, ShaderType type) { return NULL; }



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
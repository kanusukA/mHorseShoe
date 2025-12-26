#pragma once


// Local
#include <GDHandler/stuff/Stuff.h>
#include <feel/Feel.h>
// Third

// STL



class Scene;
class Case;

class ResourceHandler;
class Monster;
class Kint;
class Feel;



// Primary Class.
// Used to build object with all required contexts and initializations
class GDBuilderContext
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
	}

	virtual void update(float deltaTime) {}

	PxRigidDynamic* getPxRigidDynamic(std::string name_p, PxTransform transform, PxGeometry* geometry, float mass);
	PxRigidStatic* getPxRigidStatic(std::string name_p, PxTransform transform, PxGeometry* geometry);
	
	// Create Scene
	virtual Ogre::SceneNode* createScene(std::string name_p) { return nullptr; }
	virtual Ogre::Entity* createObject(std::string name_p) { return nullptr; }
	virtual Ogre::Mesh* createRenderMesh(std::string meshName) { return nullptr; }

};

//class GDBuilderImpli {
//private:
//	GDBuilderContext* builderCxt;
//public:
//	GDBuilderImpli(GDBuilderContext* builderCxt_p) {
//		builderCxt = builderCxt_p;
//	}
//};



// function

Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation) {
	return Ogre::Quaternion(orientation.w, orientation.x, orientation.y, orientation.z);
}
Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat) {
	return Ogre::Vector4(quat.w,quat.x,quat.y,quat.z);
}
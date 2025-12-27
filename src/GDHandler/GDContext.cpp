#include "GDContext.h"

void GDBuilderContext::startEngine()
{
	// Start Ogre Rendering engine and inits SdlWindow
	monster->InitMonster();

	// Starts Sdl Window for key, mouse and other inputs
	feel->initFeel(monster->sdlWindow,this);
	

	// Start PhysX
	kint->InitPhysics();


}

PxRigidDynamic* GDBuilderContext::getPxRigidDynamic(std::string name_p,PxTransform transform, PxGeometry* geometry, float mass)
{
	return kint->createRigidBody(name_p, transform, mass, geometry);
}

PxRigidStatic* GDBuilderContext::getPxRigidStatic(std::string name_p, PxTransform transform, PxGeometry* geometry)
{
	return kint->createStaticBody(name_p, transform, geometry);
}


// function

Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation) {
	return Ogre::Quaternion(orientation.w, orientation.x, orientation.y, orientation.z);
}
Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat) {
	return Ogre::Vector4(quat.w, quat.x, quat.y, quat.z);
}
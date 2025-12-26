#include "GDContext.h"

void GDBuilderContext::startEngine()
{
	// Start Ogre Rendering engine and inits SdlWindow
	monster->InitMonster();

	// Starts Sdl Window for key, mouse and other inputs
	feel->initFeel(monster->sdlWindow);

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

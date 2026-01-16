#include "GDContext.h"

void GDBuilderContext::startEngine()
{	
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

void GDBuilderContext::monSetLocation(std::filesystem::path parentPath_p, std::string OgreGroup)
{
	monster->addOgreResourceLocation(parentPath_p.string(), OgreGroup);
}

Ogre::MeshPtr GDBuilderContext::monGetMesh(std::string meshName_p)
{
	return monster->getMesh(meshName_p, OGRE_MESH_GROUP);
}

Ogre::Entity* GDBuilderContext::monCreateEntity(std::string name_p, Ogre::MeshPtr mesh_p)
{
	return monster->createEntity(name_p,mesh_p)
}

bool GDBuilderContext::sceneExists(std::string scnName_p)
{
	return monster->oScnManager->hasSceneNode(scnName_p);
}

bool GDBuilderContext::objectExists(std::string objName_p)
{
	return monster->oScnManager->hasEntity(objName_p);
}





// function

Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation) {
	return Ogre::Quaternion(orientation.w, orientation.x, orientation.y, orientation.z);
}
Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat) {
	return Ogre::Vector4(quat.w, quat.x, quat.y, quat.z);
}
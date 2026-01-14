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

bool GDBuilderContext::sceneExists(std::string scnName_p)
{
	return monster->oScnManager->hasSceneNode(scnName_p);
}

bool GDBuilderContext::objectExists(std::string objName_p)
{
	return monster->oScnManager->hasEntity(objName_p);
}



Ogre::MeshPtr GDBuilderContext::createMesh(std::string meshName_p, std::string OgreGroup /*= OGRE_MESH_GROUP*/)
{
	return monster->getMesh(meshName_p, OgreGroup);
}

void GDBuilderContext::removeSceneNode(Ogre::SceneNode* scnNode_p)
{
	monster->deleteScnNode(scnNode_p);
}

// function

Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation) {
	return Ogre::Quaternion(orientation.w, orientation.x, orientation.y, orientation.z);
}
Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat) {
	return Ogre::Vector4(quat.w, quat.x, quat.y, quat.z);
}
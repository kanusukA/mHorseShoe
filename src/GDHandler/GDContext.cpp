#include "GDContext.h"

void GDBuilderContext::startEngine()
{	
	// Start PhysX
	kint->InitPhysics();

	// Start Resource Group initialization
	std::vector<std::filesystem::path>* MaterialPaths = ResourceHandler::GetInstance()->getMaterialsLoaded();
	
	// Material
	for (int index = 0; index < MaterialPaths->size(); index++)
	{
		monster->addOgreResourceLocation(MaterialPaths->at(index).parent_path().string(), OGRE_MATERIAL_GROUP);
	}
	monster->initalizeResourceGroup(OGRE_MATERIAL_GROUP);
	
	//monster->setGrid();
	// implement saving and loading

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
	return monster->createEntity(name_p, mesh_p);
}

Ogre::MaterialPtr GDBuilderContext::monCreateMaterial(std::string name_p)
{
	return monster->getMaterial(name_p, OGRE_MATERIAL_GROUP);
}

Ogre::MaterialPtr GDBuilderContext::monCreateNewMaterial(std::string name_p)
{
	Ogre::MaterialPtr mat_p =  monster->createEmptyMaterial(name_p, OGRE_MATERIAL_GROUP);
	if (!mat_p)
	{
		return monster->getMaterial(name_p, OGRE_MATERIAL_GROUP);
	}
	return mat_p;
}

Ogre::SceneNode* GDBuilderContext::monCreateSceneNode(std::string name_p, Ogre::SceneNode* parentNode_p)
{
	return monster->createNewScnNodeAttach(name_p, parentNode_p);
}

void GDBuilderContext::monDeleteSceneNode(Ogre::SceneNode* scene_p)
{
	scene_p->detachAllObjects();

	scene_p->destroyAllChildrenAndObjects();

	monster->oScnManager->destroySceneNode(scene_p);

}

void GDBuilderContext::monDeleteEntity(Ogre::Entity* entity_p)
{
	if (entity_p)
	{
		monster->oScnManager->destroyEntity(entity_p);
	}
	
}



RSUS* GDBuilderContext::monProvideRsus()
{
	return RSUS::GetInstance();
}

void GDBuilderContext::monSetShaderVars(std::vector<ShaderVar> vars_p, Ogre::GpuProgramParametersPtr shaderPrgm)
{
	RSUS::GetInstance()->updateShaderVars(vars_p, shaderPrgm);
}

bool GDBuilderContext::sceneExists(std::string scnName_p)
{
	//return monster->oScnManager->hasSceneNode(scnName_p);
	return false;
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
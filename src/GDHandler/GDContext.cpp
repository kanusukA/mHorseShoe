#include "GDContext.h"

void GDBuilderContext::startEngine()
{	
	
	//loadMaterialsDpToOgre();

	monInitalizeAllResourceGroups();

	monster->setShadowTechnique();

}


void GDBuilderContext::loadMaterialsDpToOgre()
{
	std::vector<std::filesystem::path>* MaterialPaths = ResourceHandler::GetInstance()->getMaterialsLoaded();

	// Material
	for (int index = 0; index < MaterialPaths->size(); index++)
	{
		monster->addOgreResourceLocation(MaterialPaths->at(index).parent_path().string(), OGRE_MATERIAL_GROUP);
	}
	monster->initalizeResourceGroup(OGRE_MATERIAL_GROUP);
}

// Ogre must be initalized before this!
void GDBuilderContext::monInitalizeAllResourceGroups()
{
	monster->addResourceGroup(OGRE_MESH_GROUP);
	//// Meshes
	std::vector<std::filesystem::path>* paths = ResourceHandler::GetInstance()->fetchResourceGroupVecByName(ResourceGroup::ResourceMasterGroups.at(ResourceGroup::RENDER_MESH_PATH));
	if (paths)
	{
		for (size_t i = 0; i < paths->size(); i++) {
			monAddLocation(paths->at(i).parent_path(), OGRE_MESH_GROUP);
		}
		monInitializeResourceGroup(OGRE_MESH_GROUP);
		paths->clear();
	}
	monster->addResourceGroup(OGRE_MATERIAL_GROUP);
	//// Material - Material Paths are sensitive, make sure the internal material name is not similar in the dir.
	paths = ResourceHandler::GetInstance()->fetchResourceGroupVecByName(ResourceGroup::ResourceMasterGroups.at(ResourceGroup::MATERIAL_PATH));
	if (paths)
	{
		for (size_t i = 0; i < paths->size(); i++) {
			monAddLocation(paths->at(i).parent_path(), OGRE_MATERIAL_GROUP);
		}
		monInitializeResourceGroup(OGRE_MATERIAL_GROUP);
		paths->clear();
	}
	
}

void GDBuilderContext::monAddLocation(std::filesystem::path parentPath_p, std::string OgreGroup)
{
	monster->addOgreResourceLocation(parentPath_p.string(), OgreGroup);
}

void GDBuilderContext::monAddResourceGroup(std::string groupName)
{
	monster->addResourceGroup(groupName);
}

void GDBuilderContext::monInitializeResourceGroup(std::string groupName)
{
	monster->initalizeResourceGroup(groupName);

}

Ogre::MeshPtr GDBuilderContext::monGetMesh(std::string meshName_p)
{
	/*std::filesystem::path* path_p = ResourceHandler::GetInstance()->fetchFileInGroup(meshName_p, ResourceGroup::ResourceMasterGroups.at(ResourceGroup::RENDER_MESH_PATH)); 
	
	return monster->getMesh(path_p->string(), OGRE_MESH_GROUP);*/
	return monster->getMesh(meshName_p, OGRE_MESH_GROUP);
}

//Ogre::MeshPtr GDBuilderContext::monGetMesh(std::filesystem::path path)
//{
//	return Ogre::MeshPtr();
//}

Ogre::Entity* GDBuilderContext::monCreateEntity(std::string name_p, Ogre::MeshPtr mesh_p)
{
	return monster->createEntity(name_p, mesh_p);
}

Ogre::TexturePtr GDBuilderContext::monGetTexture(std::string textureName)
{
	return monster->getImageTexture(textureName, OGRE_TEXTURE_GROUP);
}

Ogre::MaterialPtr GDBuilderContext::monCreateMaterial(std::string name_p)
{
	return monster->getMaterial(name_p, OGRE_MATERIAL_GROUP);
}

Ogre::MaterialPtr GDBuilderContext::monCreateNewMaterial(std::string name_p)
{
	return monster->createEmptyMaterial(name_p, OGRE_MATERIAL_GROUP);
}

Ogre::SceneNode* GDBuilderContext::monCreateSceneNode(std::string name_p, Ogre::SceneNode* parentNode_p)
{
	return monster->createNewScnNodeAttach(name_p, parentNode_p);
}

void GDBuilderContext::monDeleteSceneNode(Ogre::SceneNode* scene_p)
{
	monster->rmSceneNode(scene_p->getName());
}

void GDBuilderContext::monDeleteEntity(Ogre::Entity* entity_p)
{
	monster->rmEntity(entity_p->getName());
	
}

void GDBuilderContext::monDeleteMaterial(Ogre::MaterialPtr material_p)
{
	monster->rmMaterial(material_p->getName(), OGRE_MATERIAL_GROUP);
}

RSUS* GDBuilderContext::monProvideRsus()
{
	return RSUS::GetInstance();
}

ImguiFonts* GDBuilderContext::monProvideFonts()
{
	return monster->fonts;
}

void GDBuilderContext::monSetShaderVars(std::vector<ShaderVar> vars_p, Ogre::GpuProgramParametersPtr shaderPrgm)
{
	RSUS::GetInstance()->updateShaderVars(vars_p, shaderPrgm);
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
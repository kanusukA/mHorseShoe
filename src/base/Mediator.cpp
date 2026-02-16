#include "Mediator.h"

SelectedCase* ModelComponent::selectedCase;
SelectedMaterial* ModelComponent::selectedMaterial;
SelectedScene* ModelComponent::selectedScene;
SelectedObject* ModelComponent::selectedObject;

std::vector<std::filesystem::path>* ModelComponent::meshDpVec;
std::vector<std::filesystem::path>* ModelComponent::materialDpVec;
std::vector<std::filesystem::path>* ModelComponent::shaderDpVec;
std::vector<std::filesystem::path>* ModelComponent::textureDpVec;

std::vector<Ogre::TexturePtr>* ModelComponent::imageTextures;

std::vector<std::shared_ptr<Case>>* ModelComponent::caseVec;

std::vector<std::filesystem::path>* ModelComponent::savedCaseFiles;

void ModelComponent::refreshImageTextures()
{
	for (int i = 0; i < textureDpVec->size(); i++) {
		
		if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(OGRE_TEXTURE_GROUP))
		{
			Ogre::ResourceGroupManager::getSingleton().createResourceGroup(OGRE_TEXTURE_GROUP);
		}
		
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().load(textureDpVec->at(i).string().c_str(), OGRE_TEXTURE_GROUP);
		if (tex)
		{
			imageTextures->push_back(tex);
		}
		else
		{
			ToastComponent::GetInstance()->addMessage("Error Loading Texture");
		}
		

		
	}
}

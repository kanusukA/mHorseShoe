#include "Material.h"


ResID MaterialManager::createMaterialResource(std::string mat_name)
{
	MaterialResource mat = MaterialResource(); // Destroyed by the end of the function
	mat.setName(mat_name);
	return resourceHan->createMaterial(mat); // Creates a unique_ptr from it
}

Material* MaterialManager::fetchNewMaterial(ResID materialID, Ogre::MaterialPtr matPtr)
{
	MaterialResource* fetchedMaterial = resourceHan->fetchNewMaterial(materialID);

	if (fetchedMaterial)
	{
		Material* mat = new Material(*fetchedMaterial);
		mat->initMaterial(matPtr);
		delete fetchedMaterial;
		return mat;

	}
	else {
		ToastComponent::GetInstance()->addMessage("Failed to fetch material , ID : " + std::to_string(materialID));
	}
}


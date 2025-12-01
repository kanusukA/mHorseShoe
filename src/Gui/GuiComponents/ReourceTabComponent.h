#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class ResourceTabModelComponent : public ModelComponent {
public:

	std::vector<std::filesystem::path>* renderMeshes;
	std::vector<std::filesystem::path>* colliderMeshes;
	std::vector<std::filesystem::path>* meshMaterials;
	std::vector<std::filesystem::path>* images;
	Ogre::StringVectorPtr ogreMeshes;

	ResourceTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override{
		renderMeshes = gdSource->getResourceHandler()->renderMeshes;
		colliderMeshes = gdSource->getResourceHandler()->colliderMeshes;
		meshMaterials = gdSource->getResourceHandler()->meshMaterials;
		images = gdSource->getResourceHandler()->images;
		ogreMeshes = gdSource->getResourceHandler()->ogreRenderMeshes;

	}

	void RenderMeshToOgreBtn() {
		gdSource->getResourceHandler()->addOgreRenderMeshResourceLocation();
	}

	void FetchAllResourcesBtn() {
		gdSource->getResourceHandler()->getAllResources();
	}


};

class ResourceTabComponent : public ViewComponent
{
private:
	ResourceTabModelComponent* resourceTabModel;

public:
	ResourceTabComponent(const char* name_p,ResourceTabModelComponent* recModel) :
		ViewComponent(name_p) {
		resourceTabModel = recModel;
	}

	void view() override;

};


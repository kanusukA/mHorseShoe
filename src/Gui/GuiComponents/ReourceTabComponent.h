#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

#include <imgui_stdlib.h>

class ResourceTabModelComponent : public ModelComponent {
public:

	std::vector<std::filesystem::path>* renderMeshes;
	std::vector<std::filesystem::path>* colliderMeshes;
	std::vector<std::filesystem::path>* meshMaterials;
	std::vector<std::filesystem::path>* images;
	Ogre::StringVectorPtr ogreMeshes;

	// PATHS
	std::vector<std::string>* paths;

	std::string* inputPath = new std::string("");

	int edit = -1;

	ResourceTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override{
		renderMeshes = gdSource->getResourceHandler()->renderMeshes;
		colliderMeshes = gdSource->getResourceHandler()->colliderMeshes;
		meshMaterials = gdSource->getResourceHandler()->meshMaterials;
		images = gdSource->getResourceHandler()->images;
		ogreMeshes = gdSource->getResourceHandler()->ogreRenderMeshes;

		paths = gdSource->getResourceHandler()->getPaths();

	}

	void RenderMeshToOgreBtn() {
		gdSource->getResourceHandler()->addOgreRenderMeshResourceLocation();
	}

	void FetchAllResourcesBtn() {
		gdSource->getResourceHandler()->getAllResources();
	}

	void editPath(int pathPos) {
		*inputPath = paths->at(pathPos);
		edit = pathPos;
	}
	void setPath() {
		try
		{
			this->gdSource->getResourceHandler()->setPath(*inputPath, edit);
		}
		catch (...)
		{
			ToastComponent::GetInstance()->addMessage("Invalid path - " + *inputPath);
		}
		
		edit = -1;
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


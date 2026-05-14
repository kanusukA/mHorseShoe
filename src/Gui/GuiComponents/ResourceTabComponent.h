#pragma once

#include <base/Mediator.h>


//#include <imgui_stdlib.h>

class ResourceTabModelComponent : public ModelComponent {
public:

	std::vector<std::filesystem::path>* renderMeshes;
	std::vector<std::filesystem::path>* materials;
	std::vector<std::filesystem::path>* images;
	std::vector<std::filesystem::path>* shaders;


	// PATHS
	std::vector<std::string>* paths;

	//Load paths
	std::vector<std::string>* loadPaths;

	std::string* inputPath = new std::string("");

	int edit = -1;

	int editLoadPathpos = -1;

	ResourceTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override{
		renderMeshes = gdSource->getResourceHandler()->getRenderMeshLoaded();
		//colliderMeshes = gdSource->getResourceHandler()->getColliderMeshLoaded();
		materials = gdSource->getResourceHandler()->getMaterialsLoaded();
		images = gdSource->getResourceHandler()->getTexturesLoaded();
		shaders = gdSource->getResourceHandler()->getShadersLoaded();

		paths = gdSource->getResourceHandler()->getPaths();
		loadPaths = gdSource->getResourceHandler()->getLoadPaths();

	}

	void RenderMeshToOgreBtn() {
		//gdSource->getResourceHandler()->addOgreRenderMeshResourceLocation();
	}

	void FetchAllResourcesBtn() {
		gdSource->getResourceHandler()->getAllResources();
	}

	void editPath(int pathPos) {
		*inputPath = paths->at(pathPos);
		edit = pathPos;
	}

	void addPath(int pathPos) {
		std::string path = this->openFolderSelection();
		ToastComponent::GetInstance()->addMessage("Selected Path - " + path);
		try
		{
			loadPaths->at(pathPos) = path;
		}
		catch (...)
		{
			ToastComponent::GetInstance()->addMessage("Invalid path - " + *inputPath);
		}
	}

	void editLoadPath(int pathPos) {
		*inputPath = loadPaths->at(pathPos);
		editLoadPathpos = pathPos;
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

	void setLoadPath() {
		try
		{
			this->gdSource->getResourceHandler()->addLoadPath(ResourceLoaderEnums::ResourceLoadPaths(editLoadPathpos), *inputPath);
		}
		catch (...)
		{
			ToastComponent::GetInstance()->addMessage("Invalid path - " + *inputPath);
		}

		editLoadPathpos = -1;
	}

	void saveLoadPaths() {
		this->gdSource->getResourceHandler()->saveLoadPaths();
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


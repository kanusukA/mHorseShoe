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
	std::vector<ResourceLoadPath>* loadPaths;

	std::string emptyLoadPathName = "";
	std::string emptyLoadPaths = "";
	std::string emptyLoadPathExtensions = "";
	bool showLoadPath = false;

	std::string* inputPath = new std::string("");

	int edit = -1;

	int editLoadPathpos = -1;

	ResourceTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
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

	void editLoadPath(int pathPos) {
		//*inputPath = loadPaths->at(pathPos);
		//editLoadPathpos = pathPos;
	}

	void addPathTo(std::string* output) {
		std::string path =  this->openFolderSelection();
		*output = path;
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

	void addLoadPath() {
		ResourceLoadPath loadPath = ResourceLoadPath();
		loadPath.paths = new std::vector<std::string>();
		loadPath.extensions = new std::vector<std::string>();
		if (showLoadPath == false) {
			showLoadPath = true;
		}
		else {
			if (emptyLoadPathName.empty())
			{
				ToastComponent::GetInstance()->addMessage("Load Path not added : Add Name!");
				return;
			}

			loadPath.pathGroupName = emptyLoadPathName;

			// check paths
			std::string path = "";
			for (size_t i = 0; i < emptyLoadPaths.size(); i++)
			{
				if (emptyLoadPaths.at(i) != ',')
				{
					path += emptyLoadPaths.at(i);
				}
				else {
					if (this->gdSource->getResourceHandler()->fileExists(path))
					{
						loadPath.paths->push_back(path);
					}
					else {
						ToastComponent::GetInstance()->addMessage("Invalid File path : " + path);
					}
					path = "";
				}
			}

			if (loadPath.paths->size()  == 0)
			{
				ToastComponent::GetInstance()->addMessage("Load Path not added : Add Paths!");
				return;
			}

			//extensions
			std::string extension = "";
			for (size_t i = 0; i < emptyLoadPathExtensions.size(); i++)
			{
				if (emptyLoadPathExtensions.at(i) != ',')
				{
					extension += emptyLoadPathExtensions.at(i);
				}
				else {

					loadPath.extensions->push_back(extension);
					extension = "";
					
				}
			}

			this->gdSource->getResourceHandler()->addLoadPath(loadPath);
			emptyLoadPathExtensions = "";
			emptyLoadPathName = "";
			emptyLoadPaths = "";
			showLoadPath = false;

		}
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
		//this->gdSource->getResourceHandler()->saveLoadPaths();
	}

};

class ResourceTabComponent : public ViewComponent
{
private:
	ResourceTabModelComponent* resourceTabModel;

public:
	ResourceTabComponent(const char* name_p, ResourceTabModelComponent* recModel) :
		ViewComponent(name_p) {
		resourceTabModel = recModel;
	}

	void view() override;

};


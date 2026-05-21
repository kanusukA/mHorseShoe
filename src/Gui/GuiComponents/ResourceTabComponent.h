#pragma once

#include <base/Mediator.h>


//#include <imgui_stdlib.h>

class ResourceTabModelComponent : public ModelComponent {
public:

	std::vector<std::filesystem::path>* renderMeshes;
	std::vector<std::filesystem::path>* materials;
	std::vector<std::filesystem::path>* images;
	std::vector<std::filesystem::path>* shaders;
	
	int selectedMasterGroup = 0;

	

	// PATHS
	std::vector<std::string>* paths;

	std::vector<ResourceMasterGroup*>* masterResourceVector;

	

	//Load paths
	std::vector<ResourceLoadPath>* loadPaths;

	//std::vector<ResourceLoadPath>* emptyLoadPaths;

	std::string emptyLoadPathName = "";
	std::string emptyLoadPaths = "";
	std::string emptyLoadPathExtensions = "";
	bool showAddLoadPath = false;

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
		masterResourceVector = gdSource->getResourceHandler()->getMasterResourceVector();

	}

	std::string openFolderSelection() {
		return gdSource->openFolderSelectionDialog();
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
		if (showAddLoadPath)
		{
			ResourceLoadPath newLoadPath = ResourceLoadPath();
			newLoadPath.paths = new std::vector<std::string>();
			newLoadPath.extensions = new std::vector<std::string>();

			if (emptyLoadPathName.empty())
			{
				ToastComponent::GetInstance()->addMessage("Load path name cannot be empty!");
				return;
			}

			newLoadPath.pathGroupName = emptyLoadPathName;

			newLoadPath.masterGroupName = ResourceMasterGroups.at(selectedMasterGroup);

			// PATHS
			if (emptyLoadPaths.empty())
			{
				ToastComponent::GetInstance()->addMessage("Load path cannot be empty!");
				return;
			}
			else {
				std::string path = "";
				for (size_t i = 0; i < emptyLoadPaths.size(); i++)
				{
					if (emptyLoadPaths.at(i) != ',')
					{
						path += emptyLoadPaths.at(i);
					}
					else {
						if (!path.empty() && gdSource->getResourceHandler()->fileExists(path))
						{
							newLoadPath.paths->push_back(path);
						}
						else {
							ToastComponent::GetInstance()->addMessage("Invalid path - " + path);
						}
						path = "";
					}
				}
			}
			if (newLoadPath.paths->empty())
			{
				ToastComponent::GetInstance()->addMessage("No valid paths added!");
				return;
			}

			//EXTENSIONS
			if (emptyLoadPathExtensions.empty())
			{
				ToastComponent::GetInstance()->addMessage("Load path extensions cannot be empty!");
				return;
			}
			else {
				std::string extension = "";
				for (size_t i = 0; i < emptyLoadPathExtensions.size(); i++)
				{
					if (emptyLoadPathExtensions.at(i) != ',')
					{
						extension += emptyLoadPathExtensions.at(i);
					}
					else {
						if (!extension.empty())
						{
							newLoadPath.extensions->push_back(extension);
						}
						else {
							ToastComponent::GetInstance()->addMessage("Invalid extension - " + extension);
						}
						extension = "";
					}
				}
				if (!extension.empty())
				{
					newLoadPath.extensions->push_back(extension);
				}
			}
			
			if (newLoadPath.extensions->empty())
			{
				ToastComponent::GetInstance()->addMessage("No valid extensions added!");
				return;
			}
			

			gdSource->getResourceHandler()->addLoadPath(newLoadPath);


			showAddLoadPath = false;
			emptyLoadPathName = "";
			emptyLoadPaths = "";
			emptyLoadPathExtensions = "";

		}
		else
		{
			showAddLoadPath = true;

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


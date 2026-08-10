#include "ResourceHandler.h"


// Multi-Thread shit
ResourceHandler* ResourceHandler::pinstance_{ nullptr };
std::mutex ResourceHandler::mutex_;






ResourceHandler::ResourceHandler()
{
	ToastComponent::GetInstance()->addMessage("Starting Resource Handler");
	this->SourceDir = getSourceDir();
	ToastComponent::GetInstance()->addMessage("Source Dir : " + this->SourceDir.string());

	// Load contents in load_path
	this->initResourceLoader(&ini, SourceDir, SourceDir.string() + DATA_DIRECTORY, this->SourceDir.string() + RESOURCELOADER_DATA);

	// SETUP MASTER RESOURCE VECTOR
	this->setMasterLoadPaths();
	
	this->ini.SetUnicode();
	this->initResourceSaver(&ini, this->SourceDir.string() + DATA_DIRECTORY);

}


std::filesystem::path ResourceHandler::find(std::string fileName, std::string location)
{

	if (location.empty()) {
		throw ResourceHandlerInvalidRequest();
	}
	
	try {
		for (const auto& entry : fs::directory_iterator(location)) {

			if (entry.path().filename().string() == fileName) {
				std::cout << entry.path().filename() << " - found.";
				return entry.path();
			}
		}
	}
	catch (const std::exception& e) {
		throw ResourceHandlerFileNotFound();
	}
		
}


ResourceHandler* ResourceHandler::GetInstance() {

	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new ResourceHandler();
		pinstance_->ini.SetUnicode();

	}
	return pinstance_;


}



void ResourceHandler::saveLoadPaths() {
	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();
	this->saveResourceLoadPaths(loadPaths, this->SourceDir.string() + RESOURCELOADER_DATA);
}

void ResourceHandler::setMasterLoadPaths()
{
	

	masterResourceVector->clear();
	for (size_t i = 0; i < ResourceGroup::ResourceMasterGroups.size(); i++)
	{
		masterResourceVector->push_back(new ResourceMasterGroup(ResourceGroup::ResourceMasterGroups.at(i)));
	}

	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();

	if (!loadPaths || loadPaths->empty())
	{
		ToastComponent::GetInstance()->addMessage("No Load Paths to set in Master Resource Vector");
		return;
	}

	for (size_t i = 0; i < loadPaths->size(); i++)
	{
		
		for (size_t j = 0; j < masterResourceVector->size(); j++)
		{
			if (loadPaths->at(i).masterGroupName == masterResourceVector->at(j)->GroupName)
			{
				masterResourceVector->at(j)->loadPath = loadPaths->at(i).pathGroupName;
				masterResourceVector->at(j)->loadPathIndex = i;
			}
		}
	}

	syncMasterLoadPaths();


}

// Syncs the load_paths internal path! If load_path vector itself is changed then run setMasterLoadPath()!
void ResourceHandler::syncMasterLoadPaths()
{
	std::vector<ResourceLoadPath>* loadPaths = this->getLoadPaths();
	for (size_t i = 0; i < masterResourceVector->size(); i++)
	{
		if (masterResourceVector->at(i)->loadPath.empty())
		{
			continue;
		}
		if (masterResourceVector->at(i)->loadPathIndex < loadPaths->size() && masterResourceVector->at(i)->loadPath == loadPaths->at(masterResourceVector->at(i)->loadPathIndex).pathGroupName)
		{
			for (size_t pathsIndex = 0; pathsIndex < loadPaths->at(masterResourceVector->at(i)->loadPathIndex).paths->size(); pathsIndex++)
			{
				for (size_t extIndex = 0; extIndex < loadPaths->at(masterResourceVector->at(i)->loadPathIndex).extensions->size(); extIndex++)
				{
					fetchPathContents(loadPaths->at(masterResourceVector->at(i)->loadPathIndex).paths->at(pathsIndex), 
						loadPaths->at(masterResourceVector->at(i)->loadPathIndex).extensions->at(extIndex), masterResourceVector->at(i)->ResourcePaths,true);
				}
			}
		}
	}


}



std::vector<std::filesystem::path>* ResourceHandler::fetchResourceGroupVecByName(std::string groupName)
{
	for (size_t i = 0; i < masterResourceVector->size(); i++)
	{
		if (masterResourceVector->at(i)->GroupName == groupName)
		{
			return masterResourceVector->at(i)->ResourcePaths;
		}
	}
	return nullptr;
}

std::filesystem::path* ResourceHandler::fetchFileInGroup(std::string filename_p, std::string group_p)
{
	std::vector<std::filesystem::path>* groupVec = this->fetchResourceGroupVecByName(group_p);
	if (groupVec)
	{
		for (size_t i = 0; i < groupVec->size(); i++)
		{
			if (groupVec->at(i).filename().string() == filename_p || groupVec->at(i).stem().string() == filename_p)
			{
				return &groupVec->at(i);
			}
		}
	}
	return nullptr;
}

std::vector<std::filesystem::path>* ResourceHandler::fetchResourceGroupVecByIndex(int masterIndex)
{
	if (masterResourceVector && masterIndex < masterResourceVector->size())
	{
		return masterResourceVector->at(masterIndex)->ResourcePaths;
	}
	return nullptr;
}

std::filesystem::path ResourceHandler::getSourceDir()
{
	
	WCHAR path[MAX_PATH];

	GetModuleFileNameW(NULL, path, MAX_PATH);

	char strPath[MAX_PATH];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, path, -1, strPath, MAX_PATH, &DefChar, NULL);
	std::filesystem::path srcPath = strPath;
	
	return srcPath.remove_filename();

}










#include "ResourceLoader.h"

void ResourceLoader::loadSavedPaths()
{
	ini->Reset();
	ini->LoadFile(this->loadLocationIniPath.c_str());

	CSimpleIniA::TNamesDepend loadPaths;
	ini->GetAllKeys("LoadPaths", loadPaths);


	for (const auto& entry : loadPaths )
	{
		try
		{
			int pos = std::stoi(entry.pItem);

			std::string value = ini->GetValue("LoadPaths", entry.pItem);

			if (!value.empty())
			{
				if (pos < 7)
				{
					load_paths->at(pos) = value;
				}
				else {
					load_paths->push_back(value);
				}
			}
			

		}
		catch (...)
		{
			std::cout << "Error loading ResourceSaved Path : " << entry.pItem << std::endl;

		}
		
			
		
		
	}

}

void ResourceLoader::saveLoadPaths()
{
	ini->Reset();

	for (int i = 0; i < this->load_paths->size(); i++)
	{
		if (!load_paths->at(i).empty())
		{
			ini->SetValue("LoadPaths",std::to_string(i).c_str(), this->load_paths->at(i).c_str());
		}
		
	}
	ini->SaveFile(this->loadLocationIniPath.c_str());

	ini->Reset();
}

void ResourceLoader::addLoadPath(ResourceLoaderEnums::ResourceLoadPaths pathType_p, std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->at(pathType_p) = path_p;
	}
	else {
		throw ResourceHandlerLoaderError("Load path SET invalid");
	}
	
}

void ResourceLoader::addLoadPath(std::string path_p)
{
	if (std::filesystem::exists(path_p))
	{
		this->load_paths->push_back(path_p);
	}
}



void ResourceLoader::loadMaterials(std::vector<std::filesystem::path>* output,std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::MaterialPath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::MaterialPath), extension, output, searchFolders);
		}
	}

}

void ResourceLoader::loadShaders(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::Shaders).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::Shaders), extension, output, searchFolders);
		}
	}
}

void ResourceLoader::loadRenderMesh(std::vector<std::filesystem::path>* output,std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::RenderMeshPath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::RenderMeshPath), extension, output, searchFolders);
		}
	}
}

void ResourceLoader::loadColliderMesh(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::ColliderMeshPath).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::ColliderMeshPath), extension, output, searchFolders);
		}
	}
}

void ResourceLoader::loadTextures(std::vector<std::filesystem::path>* output, std::string extension, bool searchAllResources, bool searchFolders, bool clearOutput)
{
	if (clearOutput)
	{
		output->clear();
	}

	if (searchAllResources)
	{
		for (int i = 0; i < load_paths->size(); i++)
		{
			if (!load_paths->at(i).empty())
			{
				fetchPathContents(load_paths->at(i), extension, output, searchFolders);
			}
		}
	}
	else {
		if (!load_paths->at(ResourceLoaderEnums::MaterialTexture).empty())
		{
			fetchPathContents(load_paths->at(ResourceLoaderEnums::MaterialTexture), extension, output, searchFolders);
		}
	}
}



void ResourceLoader::fetchPathContents(std::string path,std::string extension, std::vector<std::filesystem::path>* output, bool searchFolders)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (searchFolders && entry.is_directory())
		{
			fetchPathContents(entry.path().string(), extension, output, true);
		}
		else if (!entry.is_directory()) {
			std::cout << "Extension:" << entry.path().extension() << std::endl;
			if (entry.path().extension() == extension)
			{
				output->push_back(entry.path());
			}
		}
	}

}

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include <cons.h>

namespace fs = std::filesystem;

constexpr const char* RENDERMESH_LOCATION = "../assets/meshes/RenderMeshes";
constexpr const char* COLLIDERMESH_LOCATION = "../assets/meshes/ColliderMeshes";


struct HorseMeshResource {
	HorseMeshResource(const char* name, HorseResourceType resourceType) {
		Name = name;
		rType = resourceType;
	}
	const char* Name;
	HorseResourceType rType;

};

// Singlton Class
class ResourceHandler
{
private:
	const char* renderMeshLocation;
	const char* colliderMeshLocation;
	std::vector<HorseMeshResource> resources_mesh;

	static ResourceHandler* instancePtr;
	ResourceHandler() {}

public:
	// Delete Copy Constructor
	ResourceHandler(const ResourceHandler& obj) = delete;

	// Instance
	static ResourceHandler* getSingleton() {
		if (instancePtr == nullptr)
		{
			instancePtr = new ResourceHandler();
		}
		return instancePtr;
	}

	// Run this function to update and initialize data in Handler.
	void initResourceCheck();
	
	// current working directory (used for debugging)
	void currentDirctory();

	const char* getRenderMeshLocation() { return renderMeshLocation; }
	const char* getColliderMeshLocation() { return colliderMeshLocation; }
	std::vector<HorseMeshResource>& getMeshResources();


};


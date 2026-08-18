#pragma once

#include <stdio.h>
#include <iostream>
#include <filesystem>

#include <ResourceHandler/ResourceObjects.h>

#include <fastgltf/core.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/math.hpp>




class ResourceGltf {
public:

	std::vector<fastgltf::Asset> assets = std::vector<fastgltf::Asset>();

	fastgltf::Parser parser{};
	
	fastgltf::Asset* loadGltfFile(std::filesystem::path& path);

	fastgltf::Asset parseGltfFile(std::filesystem::path& path);

	std::vector<MeshData> generateMesh(fastgltf::Asset& asset);
	

};
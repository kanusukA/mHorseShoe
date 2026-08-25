#include <ResourceHandler/ResourceGltf.h>

fastgltf::Asset* ResourceGltf::loadGltfFile(std::filesystem::path& path)
{
	assets.push_back(parseGltfFile(path));
	return &assets.back();
}

fastgltf::Asset ResourceGltf::parseGltfFile(std::filesystem::path& path)
{
	
	 auto data = fastgltf::GltfDataBuffer::FromPath(path);

	if (data.error() != fastgltf::Error::None)
	{
		throw std::runtime_error("ERROR GETTING FILE DATA");
	}

	
	auto loadedGltf = parser.loadGltfBinary(data.get(), path.parent_path());

	if (loadedGltf.error() != fastgltf::Error::None)
	{
		throw std::runtime_error("ERROR GETTING FILE DATA");
	}


	return std::move(loadedGltf.get());
	
}



std::vector<MeshData> ResourceGltf::generateMesh(fastgltf::Asset& asset)
{

	std::vector<MeshData> meshData(static_cast<int>(asset.meshes.size()));
	int32_t meshIndex = 0;
	for (const auto& mesh: asset.meshes)
	{
		
		for (auto&& prim : mesh.primitives)
		{
			// indices
			auto& accessor = asset.accessors[prim.indicesAccessor.value()];
			meshData.at(meshIndex).indices.resize(accessor.count);
	
			size_t idx = 0;
			fastgltf::iterateAccessor<std::uint16_t>(asset, accessor, [&](std::uint16_t index) {
				meshData.at(meshIndex).indices[idx++] = index;
			});

			// vertex
			auto position = prim.findAttribute("POSITION");
			if (position)
			{
				auto& vertAccesser = asset.accessors[position->accessorIndex];
				meshData.at(meshIndex).vertices.resize(vertAccesser.count);

				fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, vertAccesser, [&](glm::vec3 pos,size_t index) {
					meshData.at(meshIndex).vertices[index].pos = pos;
				});

			}
			else {
				std::cout << "NO POSITIONAL DATA FOUND" << std::endl;
			}

			// uv
			auto uv = prim.findAttribute("TEXCOORD_0");
			if (uv != prim.attributes.end()) {
				
				fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, asset.accessors[(*uv).accessorIndex],
					[&](glm::vec2 v, size_t index) {
						meshData.at(meshIndex).vertices[index].texCoord = v;
					});
			}
			

		}
		meshIndex++;
	}

	return meshData;

}

#include <Monster.h>

#include <rapidobj.hpp>

Mesh Monster::loadMeshObj(std::filesystem::path& path)
{
	rapidobj::Result result = rapidobj::ParseFile(path);

	if (result.error || result.shapes.empty())
	{
		throw std::runtime_error("Invalid Mesh Obj Parsed");
	}

	uint32_t vertexCount = result.attributes.positions.size() / 3;

	Mesh mesh = Mesh();

	mesh.vertices = std::vector<Vertex>(vertexCount);

	for (size_t i = 0; i < vertexCount; ++i)
	{
		mesh.vertices[i].pos = glm::vec3(
			result.attributes.positions[3 * i + 0],
			result.attributes.positions[3 * i + 1],
			result.attributes.positions[3 * i + 2]
		);
		mesh.vertices[i].texCoord = glm::vec2(result.attributes.texcoords[2 * i + 0], result.attributes.texcoords[2 * i + 1]);
		mesh.vertices[i].color = glm::vec3(result.attributes.colors[3 * i + 0], result.attributes.colors[3 * i + 1], result.attributes.colors[3 * i + 2]);
	}


	mesh.indices = std::vector<uint32_t>(result.shapes.front().mesh.indices.size());

	for (size_t i = 0; i < mesh.indices.size(); i++)
	{
		mesh.indices[i] = result.shapes.front().mesh.indices[0].position_index;
	}


}



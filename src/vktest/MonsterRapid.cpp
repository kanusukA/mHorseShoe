#include <Monster.h>

#include <rapidobj.hpp>


// REFINE FUNCTION

Mesh Monster::loadMeshObj(std::filesystem::path& path)
{
	rapidobj::Result result = rapidobj::ParseFile("D:/source/repos/mHorseShoeVCmake/mHorseShoe/src/vktest/shaders/test1.obj");

	rapidobj::Triangulate(result);

	if (result.error || result.shapes.empty())
	{
		std::cerr << "Invalid OBJ : " << result.error.code.message() << std::endl;
		throw std::runtime_error("Invalid Mesh Obj Parsed");
	}

	uint32_t vertexCount = result.attributes.positions.size() / 3;

	Mesh mesh = Mesh();

	mesh.vertices = new std::vector<Vertex>();


	for (size_t i = 0; i < vertexCount; ++i)
	{

		Vertex vertex = Vertex();

		auto vec3 = glm::vec3(
			result.attributes.positions[3 * i + 0],
			result.attributes.positions[3 * i + 1],
			result.attributes.positions[3 * i + 2]
		);
		vertex.pos = vec3;

		std::cout << "pos : " << vec3.x << " " << vec3.y << " " << vec3.z << std::endl;
		mesh.vertices->push_back(vertex);
		//mesh.vertices->at(i).texCoord = glm::vec2(result.attributes.texcoords[2 * i + 0], result.attributes.texcoords[2 * i + 1]);
		
	}

	if (!result.materials.empty())
	{
		mesh.color = glm::vec4(result.materials[0].diffuse[0], result.materials[0].diffuse[1], result.materials[0].diffuse[2],1);
	}


	mesh.indices = new std::vector<uint16_t>(result.shapes.front().mesh.indices.size());

	int count = 0;

	/*for (const auto& index : result.shapes[0].mesh.indices)
	{
		int pos_index = index.position_index;

		mesh.vertices[count].pos = glm::vec3(
			result.attributes.positions[3 * pos_index + 0],
			result.attributes.positions[3 * pos_index + 1],
			result.attributes.positions[3 * pos_index + 2]
		);

		count += 1;
	}*/

	for (size_t i = 0; i < mesh.indices->size(); i++)
	{
		std::cout << "INDEX : " << result.shapes.front().mesh.indices[0].position_index << std::endl;
		mesh.indices->at(i) = static_cast<uint16_t>(result.shapes.front().mesh.indices[i].position_index);
	}

	return mesh;

}



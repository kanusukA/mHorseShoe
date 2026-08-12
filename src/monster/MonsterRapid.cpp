#include <monster/Monster.h>

#include <monster/rapidobj.hpp>


// REFINE FUNCTION

//Mesh Monster::loadMeshObj(std::filesystem::path& path)
//{
//	//std::filesystem::path filepath = std::filesystem::path("../../../src/monster/shaders/test1.obj");
//	rapidobj::Result result = rapidobj::ParseFile(std::filesystem::absolute(path).string().c_str());
//
//	rapidobj::Triangulate(result);
//
//	if (result.error || result.shapes.empty())
//	{
//		std::cerr << "Invalid OBJ : " << result.error.code.message() << std::endl;
//		throw std::runtime_error("Invalid Mesh Obj Parsed");
//	}
//
//	uint32_t vertexCount = result.attributes.positions.size() / 3;
//
//	Mesh mesh = Mesh();
//
//	mesh.vertices = new std::vector<vulkanUtils::Vertex>();
//
//
//	for (size_t i = 0; i < vertexCount; ++i)
//	{
//
//		vulkanUtils::Vertex vertex = vulkanUtils::Vertex();
//
//		auto vec3 = glm::vec3(
//			result.attributes.positions[3 * i + 0],
//			result.attributes.positions[3 * i + 1],
//			result.attributes.positions[3 * i + 2]
//		);
//		vertex.pos = vec3;
//
//		mesh.vertices->push_back(vertex);
//		//mesh.vertices->at(i).texCoord = glm::vec2(result.attributes.texcoords[2 * i + 0], result.attributes.texcoords[2 * i + 1]);
//		
//	}
//
//	if (!result.materials.empty())
//	{
//		mesh.color = glm::vec4(result.materials[0].diffuse[0], result.materials[0].diffuse[1], result.materials[0].diffuse[2],1);
//	}
//
//
//	mesh.indices = new std::vector<uint16_t>(result.shapes.front().mesh.indices.size());
//
//	int count = 0;
//
//	/*for (const auto& index : result.shapes[0].mesh.indices)
//	{
//		int pos_index = index.position_index;
//
//		mesh.vertices[count].pos = glm::vec3(
//			result.attributes.positions[3 * pos_index + 0],
//			result.attributes.positions[3 * pos_index + 1],
//			result.attributes.positions[3 * pos_index + 2]
//		);
//
//		count += 1;
//	}*/
//
//	for (size_t i = 0; i < mesh.indices->size(); i++)
//	{
//		mesh.indices->at(i) = static_cast<uint16_t>(result.shapes.front().mesh.indices[i].position_index);
//	}
//
//	return mesh;
//
//}




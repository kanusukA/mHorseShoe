#pragma once

#include <Stuffs/RenderMesh.h>
#include <Stuffs/Custom/SlugShader.h>


#include <slughorn/slughorn.hpp>


const static std::vector<glm::vec2> QuadFormat = { {0,1}, {1,1}, {1,0}, {0,0} };

struct SlugVertexUBO {
	glm::vec4 viewport;
	/*glm::vec4 pos;
	glm::vec4 tex;
	glm::vec4 jac;
	glm::vec4 ban;
	glm::vec4 col;*/
};
// Format { RGBA32F, RGBA16F, RGBA16UI, RG16UI, RGBA8, RGB32F, R32F };
typedef slughorn::Atlas::TextureData::Format SlugFormat;

static vk::Format slugToVkFormat(const SlugFormat& format) {
	switch (format) {
	case SlugFormat::RGBA32F:
		return vk::Format::eR32G32B32A32Sfloat;
		break;
	case SlugFormat::RGBA16F:
		return vk::Format::eR16G16B16A16Sfloat;
		break;
	case SlugFormat::RGBA16UI:
		return vk::Format::eR16G16B16A16Uint;
		break;
	case SlugFormat::RG16UI:
		return vk::Format::eR16G16Uint;
		break;
	case SlugFormat::RGBA8:
		return vk::Format::eR8G8B8A8Unorm;
		break;
	case SlugFormat::RGB32F:
		return vk::Format::eR32G32B32Sfloat;
		break;
	case SlugFormat::R32F:
		return vk::Format::eR32Sfloat;
		break;
	default:
		throw std::runtime_error("UNKNOW FORMAT FOUND!");
	}
}

class SlugMesh : public RenderMesh {
public:
	slughorn::Atlas fontAtlas;

	SlugVertexUBO slugVertUbo = SlugVertexUBO();

	std::vector<std::pair<uint16_t, slughorn::Atlas::Shape>> shapes{};

	std::shared_ptr<SlugShader> slugShader;

	std::vector<vulkanUtils::SlugVertex> slugVertices{};
	
	SlugMesh(GDBuilderContext* builderCxt_p, std::string name_p, std::shared_ptr<SlugShader> slugShader_p) :
		RenderMesh(builderCxt_p, name_p, "", slugShader_p) {
		slugShader = slugShader_p;

		allocatingBufferSizes = { sizeof(UniformBufferObject), sizeof(SlugVertexUBO) };
	
	}

	void initFontAtlas(const std::filesystem::path& fontPath);

	void loadRenderMesh() override {
		if (isMeshLoaded) return;
		// DOES NOT LOAD A NORMAL MESH
		setString("KanusukA");
		isMeshLoaded = true;
	}

	void _loadVertexIndexBuffers() override {
		vertexBuffer = builderCxt->monster->createGetVertexBuffer(sizeof(slugVertices.front()) * slugVertices.size(), slugVertices.data()).first;
		indexBuffer = builderCxt->monster->createGetIndexBuffer(mesh.front().indices).first;
	}

	void updateDescriptorWrite(vk::raii::Device* device) override {
		shader->_updateDescriptorWrites(device, &descritorSets, transformBuffers, vertexUboBuffer, fragBuffers); // vertex UBO is empty by default!
	}

	virtual void allocateBufferInfo(std::vector<std::vector<MonsterBuffer>>& buffers) {
		transformBuffers = buffers.at(0);
		vertexUboBuffer = buffers.at(1);
	}

	void updateSlugBuf() {

		slugVertUbo.viewport.x = builderCxt->monster->vkMonsterStats.swapChainExtent.width;
		slugVertUbo.viewport.y = builderCxt->monster->vkMonsterStats.swapChainExtent.height;

	}

	// runs and sets vertex buffer for this string
	void setString(const std::string& str) {

		int m_advance = 0;

		int index = 0;

		mesh.push_back(MeshData());
		
		for (size_t i = 0; i < str.size(); i++)
		{
			// per vertex
			for (size_t vert = 0; vert < 2; vert++)
			{
				
				
				vulkanUtils::SlugVertex slugVert = vulkanUtils::SlugVertex();

				auto shape = fontAtlas.getShape(slughorn::Key(str[i]));
				slugVert.pos.x = m_advance + (QuadFormat[vert].x); // model position x
				slugVert.pos.y = shape->height * QuadFormat[vert].y; // model position y
				slugVert.pos.z = 1.0f; // model normal x
				slugVert.pos.w = 1.0f; // model normal y

				m_advance += shape->advance;

				// location of band in texture
				// 31       16       0
				// | band Y | band X |
				// 31       16       0
				slugVert.tex.z = static_cast<uint16_t>(shape->bandTexX) | (static_cast<uint16_t>(shape->bandTexY) << 16);

				// band index and flags
				slugVert.tex.w = static_cast<char>(shape->bandMaxX) | (static_cast<char>(shape->bandMaxY) << 16);

				slugVert.tex.x = QuadFormat[vert].x;
				slugVert.tex.y = QuadFormat[vert].y;

				slugVert.jac = {1.0f, 0.0f, 0.0f, 1.0f};

				slugVert.ban = { shape->bandScaleX, shape->bandScaleY, shape->bandOffsetX, shape->bandScaleX };

				slugVert.color = { 0.5f, 0.1f, 0.1f };

				slugVertices.push_back(std::move(slugVert));
				mesh.front().indices.push_back(static_cast<uint16_t>(index));
				index += 1;


			}
			
			
		}
		
	}

	ShaderResource* getShader() override { return slugShader.get(); }


};
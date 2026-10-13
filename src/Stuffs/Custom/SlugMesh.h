#pragma once

#include <Stuffs/RenderMesh.h>
#include <Stuffs/Custom/SlugShader.h>


#include <slughorn/slughorn.hpp>



struct SlugVertexBuffer {
	glm::vec4 viewport;
	glm::vec4 pos;
	glm::vec4 tex;
	glm::vec4 jac;
	glm::vec4 ban;
	glm::vec4 col;
};
// Format { RGBA32F, RGBA16F, RGBA16UI, RG16UI, RGBA8, RGB32F, R32F };
typedef slughorn::Atlas::TextureData::Format SlugFormat;

vk::Format slugToVkFormat(const SlugFormat& format) {
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

	SlugVertexBuffer slugBuf = SlugVertexBuffer();

	std::vector<std::pair<uint16_t, slughorn::Atlas::Shape>> shapes{};

	std::shared_ptr<SlugShader> slugShader;

	SlugMesh(GDBuilderContext* builderCxt_p, std::string name_p, std::shared_ptr<SlugShader> slugShader_p) :
		RenderMesh(builderCxt_p, name_p, "", slugShader_p) {
		slugShader = slugShader_p;
	
	}

	void initFontAtlas(const std::filesystem::path& fontPath);

	void updateSlugBuf() {

		slugBuf.viewport.x = builderCxt->monster->vkMonsterStats.swapChainExtent.width;
		slugBuf.viewport.y = builderCxt->monster->vkMonsterStats.swapChainExtent.height;


		
		slugBuf.pos.x = position.x;
		slugBuf.pos.y = position.y;
		slugBuf.pos.z = 1.0f;
		slugBuf.pos.w = 1.0f;



	}

	void setString(const std::string& str) {
		MeshData data = MeshData();
		for (size_t i = 0; i < str.size(); i++)
		{
			auto shape = fontAtlas.getShape(slughorn::Key(str[i]));
			data.vertices.push_back()
		}
		
	}

};
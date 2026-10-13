#pragma once

#include <Stuffs/ShaderObject.h>


class SlugShader : public Shader {
public:

	SlugShader(GDBuilderContext* GDBuilderCxt_p, std::string name_p, ShaderType shaderType, std::filesystem::path vertPath, std::filesystem::path fragPath) :
		Shader(GDBuilderCxt_p, name_p, shaderType, vertPath, fragPath) 
	{
		vertex = new vulkanUtils::SlugVertex();
	}

	void initSlugShader(uint32_t texDataW, uint32_t texDataH, uint32_t curveDataW, uint32_t curveDataH, vk::Format formatTex, vk::Format formatCurve) {
		// TEXTURE DATA COMPLETE
		MonsterTexture texTexture = GDBuilderCxt->monster->createRawTexture(texDataW, texDataH, formatTex, vk::ImageUsageFlagBits::eStorage, vk::ImageAspectFlagBits::eColor);
		MonsterTexture curveTexture = GDBuilderCxt->monster->createRawTexture(curveDataW, curveDataH, formatCurve, vk::ImageUsageFlagBits::eStorage, vk::ImageAspectFlagBits::eColor);
		textures.push_back(std::move(texTexture));
		textures.push_back(std::move(curveTexture));

	}



};



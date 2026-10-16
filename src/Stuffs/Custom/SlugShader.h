#pragma once

#include <Stuffs/ShaderObject.h>


class SlugShader : public Shader {
public:

	SlugShader(GDBuilderContext* GDBuilderCxt_p, std::string name_p, ShaderType shaderType, std::filesystem::path vertPath, std::filesystem::path fragPath) :
		Shader(GDBuilderCxt_p, name_p, shaderType, vertPath, fragPath) 
	{
		vertex = new vulkanUtils::SlugVertex();
	}

	void initSlugShader(uint32_t texDataW, 
		uint32_t texDataH,
		vk::DeviceSize texSize,
		void* texData,
		uint32_t curveDataW, 
		uint32_t curveDataH, 
		vk::DeviceSize curveSize,
		void* curveData,
		vk::Format formatTex,
		vk::Format formatCurve
	) {
		// TEXTURE DATA COMPLETE
		MonsterTexture texTexture = MonsterTexture();
		MonsterTexture curveTexture = MonsterTexture();
		GDBuilderCxt->monster->_createTextureImage(
			texDataW,
			texDataH,
			formatTex,
			vk::ImageLayout::eShaderReadOnlyOptimal,
			vk::ImageUsageFlagBits::eSampled,
			texSize,
			texData,
			&texTexture.textureImage
		);
		GDBuilderCxt->monster->_createTextureImage(
			curveDataW,
			curveDataH,
			formatCurve,
			vk::ImageLayout::eShaderReadOnlyOptimal,
			vk::ImageUsageFlagBits::eSampled,
			curveSize,
			curveData,
			&curveTexture.textureImage
		);
		texTexture.textureImageView = GDBuilderCxt->monster->createImageView(*texTexture.textureImage, formatTex, vk::ImageAspectFlagBits::eColor);
		curveTexture.textureImageView = GDBuilderCxt->monster->createImageView(*curveTexture.textureImage, formatCurve, vk::ImageAspectFlagBits::eColor);

		texTexture.descriptorType = vk::DescriptorType::eSampledImage;
		curveTexture.descriptorType = vk::DescriptorType::eSampledImage;
		texTexture.imgLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		curveTexture.imgLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		textures.push_back(std::move(texTexture));
		textures.push_back(std::move(curveTexture));

	}



};



#include <Stuffs/Custom/SlugMesh.h>



#define SLUGHORN_FREETYPE_IMPLEMENTATION
#include <slughorn/freetype.hpp>

void SlugMesh::initFontAtlas(const std::filesystem::path& fontPath)
{
	bool result = slughorn::freetype::loadAsciiFont(fontPath, fontAtlas);
	if (result)
	{
		fontAtlas.build();
		if (fontAtlas.isBuilt())
		{
			slughorn::Atlas::TextureData texData = fontAtlas.getBandTextureData();
			slughorn::Atlas::TextureData curveData = fontAtlas.getCurveTextureData();
			slugShader->initSlugShader(texData.width, texData.height, curveData.width, curveData.height, slugToVkFormat(texData.format), slugToVkFormat(curveData.format));

			return;
		}
	}

	throw std::runtime_error("UNABLE TO LOAD FONT ATLAS");
}

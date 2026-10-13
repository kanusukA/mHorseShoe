#pragma once

#include <Stuffs/RenderMesh.h>
#include <Stuffs/Custom/SlugShader.h>


#include <slughorn/slughorn.hpp>





class SlugMesh : public RenderMesh {
public:
	slughorn::Atlas fontAtlas;

	std::vector<std::pair<uint16_t, slughorn::Atlas::Shape>> shapes{};

	std::shared_ptr<SlugShader> slugShader;

	SlugMesh(GDBuilderContext* builderCxt_p, std::string name_p, std::shared_ptr<SlugShader> slugShader_p) :
		RenderMesh(builderCxt_p, name_p, "", slugShader_p) {
		slugShader = slugShader_p;
	}

	void initFontAtlas(const std::filesystem::path& fontPath);

	void setString(const std::string& str) {
		auto shape = fontAtlas.getShape(slughorn::Key("A"));
		
	}

};
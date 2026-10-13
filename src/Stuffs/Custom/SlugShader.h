#pragma once

#include <Stuffs/ShaderObject.h>


class SlugShader : public Shader {
public:

	SlugShader(GDBuilderContext* GDBuilderCxt_p, std::string name_p, ShaderType shaderType, std::filesystem::path vertPath, std::filesystem::path fragPath) :
		Shader(GDBuilderCxt_p, name_p, shaderType, vertPath, fragPath) {}



};



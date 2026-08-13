#pragma once


#include <stdio.h>
#include <filesystem>

//#include <monster/VulkanUtils.h>
#include <GDHandler/ResourceHandler.h>

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>


class MonsterSlang {

	Slang::ComPtr<slang::IGlobalSession> globalSession;
	

public:

	MonsterSlang() {
		if (SLANG_FAILED(slang::createGlobalSession(globalSession.writeRef())))
		{
			throw std::runtime_error("Unable to create Slang Session");
		}
	}

	std::vector<vulkanUtils::Shader> shaders = std::vector<vulkanUtils::Shader>();
	

	uint32_t loadShader(std::string& shadername,std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath);

	void compileShaderFiles();

	std::vector<uint8_t> compileSlangFile(
		const std::string& shadername,
		const std::vector<char>& sourceCode, 
		const std::string& entryPointName, 
		const std::filesystem::path& shaderPath,
		Slang::ComPtr<slang::ISession> session
	);

};
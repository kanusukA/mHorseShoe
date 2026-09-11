#include <monster/MonsterBuffer.h>

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

#include <GDHandler/ResourceHandler.h>


class MonsterSlang : public MonsterBufferManager {
private:
	Slang::ComPtr<slang::IGlobalSession> globalSession;
	VulkanStatus* vkMonsterStats;

	std::pair<vk::raii::ShaderModule,vk::raii::ShaderModule> _loadShader(std::filesystem::path& vertPath, std::filesystem::path& fragPath, std::string vertName, std::string fragName );

public:
	
	std::vector<std::shared_ptr<vulkanUtils::Shader>> shaders{};

	void InitMonsterSlang(VulkanStatus* vkMonsterStats_p){
		vkMonsterStats = vkMonsterStats_p;
		if (SLANG_FAILED(slang::createGlobalSession(globalSession.writeRef())))
		{
			throw std::runtime_error("Unable to create global session");
		}
	}

	void loadShader(std::shared_ptr<vulkanUtils::Shader> shader);
	void loadShaderResource(ShaderResource* shaderResource);

	// for backwards compatability - use the ShaderResource function for future use
	std::shared_ptr<vulkanUtils::Shader> loadShader(const std::string& shadername, std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath);
	

	std::vector<uint8_t> compileSlangFile(
		const std::string& shadername,
		const std::vector<char>& sourceCode,
		const std::string& entryPointName,
		const std::filesystem::path& shaderPath,
		Slang::ComPtr<slang::ISession> session
	);

	vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;
	vk::raii::ShaderModule createShaderModule(const std::vector<uint8_t>& code) const;
	vk::raii::ShaderModule createShaderModule(const uint32_t* code, size_t codeSize) const;


};
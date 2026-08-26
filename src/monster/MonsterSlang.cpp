#include <monster/MonsterSlang.h>

void printDiagnostics(slang::IBlob* diagnostics) {
	if (diagnostics) {
		std::cerr << "SLANG ERROR : " << (const char*)diagnostics->getBufferPointer() << std::endl;
		throw std::runtime_error((const char*)diagnostics->getBufferPointer());
	}
}

std::shared_ptr<vulkanUtils::Shader> MonsterSlang::loadShader(const std::string& shadername, std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath)
{
	std::shared_ptr<vulkanUtils::Shader> shader = std::make_shared<vulkanUtils::Shader>();

	shader->vertShaderFilePath = new std::filesystem::path(vertfilepath);
	shader->fragShaderFilePath = new std::filesystem::path(fragfilepath);

	shader->vertShaderName = shadername + "_vert";
	shader->fragShaderName = shadername + "_frag";

	auto vertCodeSlang = std::vector<char>();
	auto fragCodeSlang = std::vector<char>();

	ResourceHandler::GetInstance()->readFileContents(vertfilepath, &vertCodeSlang);
	ResourceHandler::GetInstance()->readFileContents(fragfilepath, &fragCodeSlang);

	// target
	slang::TargetDesc targetDesc{};
	targetDesc.format = SLANG_SPIRV;
	targetDesc.profile = globalSession->findProfile("spirv_1_4");

	// session
	slang::SessionDesc sessionDesc{};
	sessionDesc.targets = &targetDesc;
	sessionDesc.targetCount = 1;

	Slang::ComPtr<slang::ISession> session;
	if (SLANG_FAILED(globalSession->createSession(sessionDesc, session.writeRef())))
	{
		throw std::runtime_error("UNABLE TO CREATE SLANG RUNTIME SESSION");
	}

	auto vertCodeSpv = std::move(compileSlangFile(shader->vertShaderName, vertCodeSlang, "vertMain", vertfilepath, session));
	auto fragCodeSpv = std::move(compileSlangFile(shader->fragShaderName, fragCodeSlang, "fragMain", fragfilepath, session));

	shader->vertexShader = std::move(createShaderModule(vertCodeSpv));
	shader->fragmentShader = std::move(createShaderModule(fragCodeSpv));

	shaders.push_back(std::move(shader));
	return shaders.back();
}

std::vector<uint8_t> MonsterSlang::compileSlangFile(const std::string& shadername, const std::vector<char>& sourceCode, const std::string& entryPointName, const std::filesystem::path& shaderPath, Slang::ComPtr<slang::ISession> session)
{
	Slang::ComPtr<slang::IBlob> diagnosticBlob;

	std::string sourceString(sourceCode.begin(), sourceCode.end());

	slang::IModule* module = session->loadModuleFromSourceString(
		shadername.c_str(),
		shaderPath.string().c_str(),
		sourceString.c_str(),
		diagnosticBlob.writeRef()
	);

	if (!module)
	{
		printDiagnostics(diagnosticBlob);

	}

	// find entrypoint
	Slang::ComPtr<slang::IEntryPoint> entryPoint;
	if (SLANG_FAILED(module->findEntryPointByName(entryPointName.c_str(), entryPoint.writeRef())))
	{
		throw std::runtime_error("UNABLE TO FIND ENTRY POINT IN SHADER");
	}

	// linking

	slang::IComponentType* components[] = { module, entryPoint.get() };
	Slang::ComPtr<slang::IComponentType> program;

	if (SLANG_FAILED(session->createCompositeComponentType(components, 2, program.writeRef(), diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	Slang::ComPtr<slang::IBlob> spirv;
	if (SLANG_FAILED(program->getEntryPointCode(0, 0, spirv.writeRef(), diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	const uint8_t* ptr = (const uint8_t*)spirv->getBufferPointer();
	size_t size = spirv->getBufferSize();

	return std::vector<uint8_t>(ptr, ptr + size);
}

[[nodiscard]] vk::raii::ShaderModule MonsterSlang::createShaderModule(const std::vector<char>& code) const {

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = code.size() * sizeof(char),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);

}

[[nodiscard]] vk::raii::ShaderModule MonsterSlang::createShaderModule(const std::vector<uint8_t>& code) const {

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = code.size() * sizeof(uint8_t),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);

}

[[nodiscard]] vk::raii::ShaderModule MonsterSlang::createShaderModule(const uint32_t* code, size_t codeSize) const
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = codeSize,
		.pCode = code
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);
}
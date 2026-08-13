#include <monster/MonsterSlang.h>


void printDiagnostics(slang::IBlob* diagnostics) {
	if (diagnostics) {

		throw std::runtime_error((const char*)diagnostics->getBufferPointer());
	}
}

uint32_t MonsterSlang::loadShader(std::string& shaderName,std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath)
{
	vulkanUtils::Shader shader = vulkanUtils::Shader();
	shader.vertShaderFilePath = new std::filesystem::path(vertfilepath);
	shader.fragShaderFilePath = new std::filesystem::path(fragfilepath);

	shader.vertCodeSlang = std::vector<char>();
	shader.fragCodeSlang = std::vector<char>();

	ResourceHandler::GetInstance()->readFileContents(vertfilepath, &shader.vertCodeSlang);
	ResourceHandler::GetInstance()->readFileContents(fragfilepath, &shader.fragCodeSlang);

	shaders.push_back(std::move(shader));
	return shaders.size();

}

void MonsterSlang::compileShaderFiles()
{
	// target
	slang::TargetDesc targetDesc{};
	targetDesc.format = SLANG_SPIRV;
	targetDesc.profile = globalSession->findProfile("spirv_1_4");

	// session
	slang::SessionDesc sessionDesc{};
	sessionDesc.targets = &targetDesc;
	sessionDesc.targetCount = 1;

	Slang::ComPtr<slang::ISession> session;
	if (SLANG_FAILED(globalSession->createSession(sessionDesc,session.writeRef())))
	{
		throw std::runtime_error("UNABLE TO CREATE SLANG RUNTIME SESSION");
	}

	for (auto& shader: shaders)
	{
		if (!shader.compiled)
		{
			shader.vertCodeSpv = std::move(compileSlangFile(shader.vertShadername, shader.vertCodeSlang, "vertMain", *shader.vertShaderFilePath, session));
			shader.fragCodeSpv = std::move(compileSlangFile(shader.fragShaderName, shader.fragCodeSlang, "fragMain", *shader.fragShaderFilePath, session));

			shader.compiled = true;

		}
	}

	

}

std::vector<uint8_t> MonsterSlang::compileSlangFile(
	const std::string& shadername, 
	const std::vector<char>& sourceCode, 
	const std::string& entryPointName, 
	const std::filesystem::path& shaderPath,
	Slang::ComPtr<slang::ISession> session
)
{
	// load from mem
	Slang::ComPtr<slang::IBlob> diagnosticBlob;

	slang::IModule* module = session->loadModuleFromSourceString(
		shadername.c_str(),
		shaderPath.string().c_str(),
		sourceCode.data(),
		diagnosticBlob.writeRef()
	);

	if (!module)
	{
		printDiagnostics(diagnosticBlob);

	}

	// find entrypoint
	Slang::ComPtr<slang::IEntryPoint> entryPoint;
	if (SLANG_FAILED(module->findEntryPointByName(entryPointName.c_str(),entryPoint.writeRef())))
	{

	}throw std::runtime_error("UNABLE TO FIND ENTRY POINT IN SHADER");

	// linking

	slang::IComponentType* components[] = { module, entryPoint.get()};
	Slang::ComPtr<slang::IComponentType> program;

	if (SLANG_FAILED(session->createCompositeComponentType(components,2,program.writeRef(),diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	Slang::ComPtr<slang::IBlob> spirv;
	if (SLANG_FAILED(program->getEntryPointCode(0,0,spirv.writeRef(),diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	const uint8_t* ptr = (const uint8_t*)spirv->getBufferPointer();
	size_t size = spirv->getBufferSize();

	return std::vector<uint8_t>(ptr, ptr + size);

}



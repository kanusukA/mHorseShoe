#include "Monster.h"



void Monster::keyHit(MappedKey key)
{
	if (key->keyCode == MKEY_WINDOW_GRAB)
	{
		windowGrab(!sdlStats->windowGrabed);
	}

}

void Monster::pollEvent(SDL_Event& event) {
	// polling events
	ImGui_ImplSDL3_ProcessEvent(&event);
}


void Monster::InitMonster() {
	// INITALIZES SDL AS WELL
	MonsterVulkan::InitVulkan(900, 1600);

	MonsterImgui::InitMonsterImgui();

	// Passdown Feel
	Feel::GetInstance()->addExtension(this);

	imDebugStats.mouseXrel = &Feel::GetInstance()->mouse.xRel;
	imDebugStats.mouseYrel = &Feel::GetInstance()->mouse.yRel;

	loadOtherMesh();
	loadSkyBox();
	
	//compile shaders
	MonsterSlang::compileShaderFiles();
	MonsterVulkan::compileToShaderModule();

	MonsterVulkan::loadAllMeshes();
	MonsterVulkan::loadMeshToPassObject();


	/*std::filesystem::path filepath = std::filesystem::path("../../../src/monster/shaders/test1.obj");
	*mesh = loadMeshObj(filepath);
	MonsterVulkan::loadMeshVertInd(mesh);

	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/skybox.obj");
	*skybox = loadMeshObj(skyboxPath);
	MonsterVulkan::loadMeshVertInd(skybox);*/

}

void Monster::updateMonster(glm::vec3 cameraPosition, glm::vec2 cameraRotation, float deltaTime)
{
	// FRAME BUFFER RESIZED IS SEPERATE FROM POLL EVENTS AS IT MUST RUN BEFOR RENDERING THE FRAME ELSE SWAPCHAIN CAN FAIL, as sdl poll events are run without block main thread;
	if (Feel::GetInstance()->mappedEvents.windowResize->eventState)
	{
		MonsterSDL::frameBufferResized();
	}

	// Input
	camera->processKeys(cameraPosition, deltaTime);

	if (sdlStats->windowGrabed)
	{
		camera->processMouse(cameraRotation);
	}

	// rendering
	MonsterImgui::startImguiFrame();

	// Imgui Rendering

	MonsterImgui::debugWindow();

	ImGui::EndFrame();

	MonsterImgui::renderFrame();


}


void Monster::Shutdown() {
	MonsterImgui::ShutdownImgui();
	MonsterVulkan::ShutdownVulkan();
	MonsterSDL::ShutdownSDL();
	
}


void Monster::loadSkyBox()
{
	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/sphere.glb");
	fastgltf::Asset* skyAsset = ResourceHandler::GetInstance()->loadGltfFile(skyboxPath);

	std::vector<hRes::Mesh> meshes = std::vector<hRes::Mesh>();

	ResourceHandler::GetInstance()->generateMesh(*skyAsset, &meshes);
	
	// Shader
	std::filesystem::path vertshader = "../../../src/monster/shaders/triangle_vert.slang";
	std::filesystem::path fragshader = "../../../src/monster/shaders/triangle_frag.slang";
	meshes[0].shaderIndex = MonsterSlang::loadShader("SkyShader", vertshader, fragshader);

	MonsterVulkan::importMesh(meshes.at(0));
	
}

void Monster::loadOtherMesh()
{
	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/box.glb");
	fastgltf::Asset* skyAsset = ResourceHandler::GetInstance()->loadGltfFile(skyboxPath);

	std::vector<hRes::Mesh> meshes = std::vector<hRes::Mesh>();

	ResourceHandler::GetInstance()->generateMesh(*skyAsset, &meshes);
	meshes[0].shaderIndex = 0;
	

	MonsterVulkan::importMesh(meshes.at(0));
}

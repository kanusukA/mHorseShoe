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

	createRequiredShaders();

	//loadSkyBox();
	loadOtherMesh();

	

//	MonsterVulkan::loadAllMeshes();
//	MonsterVulkan::loadMeshToPassObject();
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


void Monster::createRequiredShaders()
{
	// triangle shader
	auto vertShader = std::filesystem::path("../../../src/monster/shaders/tri_vert.slang");
	auto fragShader = std::filesystem::path("../../../src/monster/shaders/tri_frag.slang");
	loadShader("triangleShader", vertShader , fragShader);
	triangleShaderIndex = shaders.size() - 1;

}

void Monster::loadSkyBox()
{
	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/sphere_s.glb");
	fastgltf::Asset* skyAsset = ResourceHandler::GetInstance()->loadGltfFile(skyboxPath);

	MeshData meshData = ResourceHandler::GetInstance()->generateMesh(*skyAsset).front();

	// get mesh from monstervulkan
	std::weak_ptr<hRes::Mesh> mesh = createMesh();
	mesh.lock()->vertices = meshData.vertices;
	mesh.lock()->indices = meshData.indices;
	
	//load mesh
	loadMesh(0, 0);
	


}

void Monster::loadOtherMesh()
{
	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/box.glb");
	fastgltf::Asset* skyAsset = ResourceHandler::GetInstance()->loadGltfFile(skyboxPath);

	MeshData meshData = ResourceHandler::GetInstance()->generateMesh(*skyAsset).front();

	// get mesh from monstervulkan
	std::weak_ptr<hRes::Mesh> mesh = createMesh();
	mesh.lock()->vertices = meshData.vertices;
	mesh.lock()->indices = meshData.indices;

	loadMesh(0, 0);

	/*std::vector<hRes::Mesh> meshes = std::vector<hRes::Mesh>();

	ResourceHandler::GetInstance()->generateMesh(*skyAsset, &meshes);

	meshes[0].shaders.vertShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/triangle.spv");
	meshes[0].shaders.fragShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/triangle.spv");*/

	//MonsterVulkan::importMesh(meshes.at(0));
}

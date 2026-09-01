#include "Monster.h"

#include <glm/gtc/type_ptr.hpp>

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

	loadSkyBox();
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
	skyBoxImguiMenu();

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

	// skybox shader
	/*vertShader = std::filesystem::path("../../../src/monster/shaders/sky_vert.slang");
	fragShader = std::filesystem::path("../../../src/monster/shaders/sky_frag.slang");
	loadShader("skyShader", vertShader, fragShader);*/

}

void Monster::loadSkyBox()
{
	std::filesystem::path skyboxPath = std::filesystem::path("../../../src/monster/shaders/sphere_s.glb");
	fastgltf::Asset* skyAsset = ResourceHandler::GetInstance()->loadGltfFile(skyboxPath);

	MeshData meshData = ResourceHandler::GetInstance()->generateMesh(*skyAsset).front();

	// get mesh from monstervulkan
	std::shared_ptr<vulkanUtils::SkyBoxShader> sbs = std::make_shared<vulkanUtils::SkyBoxShader>();
	sbs->vertShaderName = "SKY_BOX_VERT_SHADER";
	sbs->fragShaderName = "SKY_BOX_FRAG_SHADER";
	sbs->vertShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/sky_vert.slang");
	sbs->fragShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/sky_frag.slang");
	loadShader(sbs);

	skyMesh->setShader(sbs);
	

	skyMesh->vertices = meshData.vertices;
	skyMesh->indices = meshData.indices;
	

	
	//load mesh
	addMesh(skyMesh);

	loadMeshContainingShader(0);

	// mesh is loaded!!!!!
}


void Monster::skyBoxImguiMenu()
{
	ImGui::SetNextWindowPos(ImVec2(0, 300));
	ImGui::Begin("Skybox", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);


	if (ImGui::DragFloat("highlightOffset", &skyMesh->skyBufObj.highlightOffset,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("highlightSmoothness", &skyMesh->skyBufObj.highlightSmoothness,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("middleOffset", &skyMesh->skyBufObj.middleOffset,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("middleSmoothness", &skyMesh->skyBufObj.middleSmoothness,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("coreOffset", &skyMesh->skyBufObj.coreOffset,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("coreSmoothness", &skyMesh->skyBufObj.coreSmoothness,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("bumpOffset", &skyMesh->skyBufObj.bumpOffset,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("bumprange", &skyMesh->skyBufObj.bumprange,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat("bumpHeight", &skyMesh->skyBufObj.bumpHeight,0.0005f,0.0f,1.0f))
	{
		skyMesh->updateBuffer();
	}

	if (ImGui::DragFloat4("baseColor", glm::value_ptr(skyMesh->skyBufObj.baseColor), 0.0005f, 0.0f, 1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat4("highlightColor", glm::value_ptr(skyMesh->skyBufObj.highlightColor), 0.0005f, 0.0f, 1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat4("middleColor", glm::value_ptr(skyMesh->skyBufObj.middleColor), 0.0005f, 0.0f, 1.0f))
	{
		skyMesh->updateBuffer();
	}
	if (ImGui::DragFloat4("coreColor", glm::value_ptr(skyMesh->skyBufObj.coreColor), 0.0005f, 0.0f, 1.0f))
	{
		skyMesh->updateBuffer();
	}
	

	ImGui::End();
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
	mesh.lock()->position = glm::vec3(3.0f, 0.0f, 0.0f);

	loadMesh(0, 1);

	/*std::vector<hRes::Mesh> meshes = std::vector<hRes::Mesh>();

	ResourceHandler::GetInstance()->generateMesh(*skyAsset, &meshes);

	meshes[0].shaders.vertShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/triangle.spv");
	meshes[0].shaders.fragShaderFilePath = new std::filesystem::path("../../../src/monster/shaders/triangle.spv");*/

	//MonsterVulkan::importMesh(meshes.at(0));
}

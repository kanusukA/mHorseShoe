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

	ImGui::Begin("Testing");

	ImGui::Text("TESTING");

	ImGui::End();


	ImGui::EndFrame();

	MonsterImgui::renderFrame();


}


void Monster::Shutdown() {
	MonsterImgui::ShutdownImgui();
	MonsterVulkan::ShutdownVulkan();
	MonsterSDL::ShutdownSDL();
	
}
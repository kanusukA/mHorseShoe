#include "Monster.h"
#include <cstddef>

void Monster::InitSDLWindow() {



	if (!SDL_Init(SDL_INIT_VIDEO))
	{
//		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}


	sdlWindow = SDL_CreateWindow(
		"PSYCHO",
		1600,
		800,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
	);


	if (!sdlWindow)
	{
		std::runtime_error("Unable to create SDL3 Window");
		return;
	}

	/*SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);*/


}

void Monster::grabMouse(bool grab)
{
	SDL_SetWindowRelativeMouseMode(sdlWindow, grab);
	windowGrabbed = SDL_GetWindowRelativeMouseMode(sdlWindow);
}

void Monster::hideMouse(bool hide)
{
}

void Monster::CameraStatsImGuiWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("STATS", nullptr,ImGuiWindowFlags_NoMove);

	ImGui::Text("POSITION : ");

	ImGui::Text("X : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(camera.position.x).c_str());
	ImGui::SameLine();
	ImGui::Text(" Y : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(camera.position.y).c_str());
	ImGui::SameLine();
	ImGui::Text(" Z : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(camera.position.z).c_str());

	ImGui::Text("ROTATION : ");

	ImGui::Text("PITCH : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(camera.pitch).c_str());
	ImGui::Text("YAW : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(camera.yaw).c_str());

	ImGui::End();
}





void Monster::ShutdownSDL() {
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

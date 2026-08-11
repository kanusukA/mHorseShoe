#include <monster/MonsterSDL.h>
#include <cstddef>

void MonsterSDL::InitSDLWindow(uint16_t windowHeight, uint16_t windowWidth) {
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		throw std::runtime_error(SDL_GetError());
//		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}
	sdlStats.get()->windowWidth = static_cast<int>(windowWidth);
	sdlStats.get()->windowHeight = static_cast<int>(windowHeight);

	sdlStats.get()->window = SDL_CreateWindow(
		"PSYCHO",
		sdlStats->windowWidth,
		sdlStats->windowHeight,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
	);

	if (!sdlStats->window)
	{
		throw std::runtime_error(SDL_GetError());
//		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}
}

void MonsterSDL::frameBufferResized() 
{
	sdlStats->framebufferResized = true;
	SDL_GetWindowSizeInPixels(sdlStats->window, &sdlStats->windowWidth, &sdlStats->windowHeight);

}

void MonsterSDL::windowGrab(bool state)
{
	SDL_SetWindowRelativeMouseMode(sdlStats->window, state);
	sdlStats->windowGrabed = SDL_GetWindowRelativeMouseMode(sdlStats->window);
}


void MonsterSDL::ShutdownSDL() {
	SDL_DestroyWindow(sdlStats->window);
	SDL_Quit();
}
#include <monster/MonsterSDL.h>

void MonsterSDL::InitSDLWindow(uint16_t windowHeight, uint16_t windowWidth) {
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
//		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}
	sdlStats.get()->windowWidth = windowWidth;
	sdlStats.get()->windowHeight = windowHeight;

	sdlStats.get()->window = SDL_CreateWindow(
		"PSYCHO",
		windowWidth,
		windowWidth,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
	);

	if (!sdlStats->window)
	{
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
	SDL_SetWindowMouseGrab(sdlStats->window, state);
	sdlStats->windowGrabed = SDL_GetWindowMouseGrab(sdlStats->window);
}


void MonsterSDL::ShutdownSDL() {
	SDL_DestroyWindow(sdlStats->window);
	SDL_Quit();
}
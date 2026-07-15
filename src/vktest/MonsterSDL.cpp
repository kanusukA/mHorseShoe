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
//		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);


}



void Monster::ShutdownSDL() {
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

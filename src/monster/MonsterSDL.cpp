#include "Monster.h"

void Monster::InitSDLWindow() {
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}

	sdlWindow = SDL_CreateWindow(
		"PSYCHO",
		1600,
		800,
		SDL_WINDOW_RESIZABLE
	);

	if (!sdlWindow)
	{
		ToastComponent::GetInstance()->addMessage("Unable to create SDL Window!");
		return;
	}
}



void Monster::ShutdownSDL() {
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}
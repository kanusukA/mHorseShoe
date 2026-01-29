#include "GDSystem.h"

void GDSystem::setWindowSize()
{
	SDL_GetWindowSizeInPixels(window, windowSize->width, windowSize->height);

	ToastComponent::GetInstance()->addMessage("Width : " + std::to_string(*windowSize->width));
	ToastComponent::GetInstance()->addMessage("Height : " + std::to_string(*windowSize->height));
}

void GDSystem::resized()
{
	setWindowSize();
}

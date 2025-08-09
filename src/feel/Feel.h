#pragma once

#ifndef FEEL_H
#define FEEL_H

#include <string>
#include "KeyHandler.h"
#include <Windows.h>
#include <iostream>

#include<imgui.h>

#include <stdio.h>


// Feel class handles all the userInput obtained from SDL.
// MUST HAVE INITALIZED AND RUNNING SDL3.
// Doesn't work with vulkan renderer
class Feel
{
public:
	// INITIALIZES INPUT HANDLER. CREATES NEW BINDINGS.
	void initFeel(SDL_Window* sdlWindow);

	KeyHandler* keyHandler;
	SDL_Window* window = NULL;

	void updateInput(float deltaTime);

	void hideMouse();
	void showMouse();

	bool mouseHidden = false;

	void consoleMode(bool state);

};




#endif // !FEEL_H




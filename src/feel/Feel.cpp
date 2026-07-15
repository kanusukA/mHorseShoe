#include "Feel.h"


void Feel::initFeel(SDL_Window* sdlWindow, KeyHandler* keyHandler_p)
{
	window = sdlWindow;
	SDL_SetWindowMouseGrab(window, true);

	keyHandler = keyHandler_p;
}

void Feel::hideMouse() {
//	ShowCursor(false);
	SDL_SetWindowRelativeMouseMode(window,true);
	mouseHidden = true;
}

void Feel::showMouse() {
//	ShowCursor(true);
	SDL_SetWindowRelativeMouseMode(window, false);
	mouseHidden = false;
	
}



void Feel::consoleMode(bool state)
{
	SDL_SetWindowMouseGrab(window,!state);
//	ShowCursor(state);
	keyHandler->inputKeys->CONSOL_KEY = false;
}


void Feel::updateInput(float deltaTime) {

	SDL_Event event;

	keyHandler->mouseKeys->MouseXrel = 0;
	keyHandler->mouseKeys->MouseYrel = 0;

	// keyBoard lock if occupire by ImGui
	/*if (ImGui::GetIO().WantCaptureKeyboard) {
		SDL_PollEvent(&event);
		keyHandler->inputKeys->MOUSE_LEFT_CLICK = 0;
		keyHandler->inputKeys->MOUSE_RIGHT_CLICK = 0;
		return;
	}*/

	while (SDL_PollEvent(&event) != 0)
	{
		// NEW IMPLEMENTATION
		if(event.key.type == SDL_EVENT_KEY_DOWN || event.key.type == SDL_EVENT_KEY_UP){
			for (int i = 0; i < keyHandler->keys->size(); i++)
			{
				if(event.key.key == keyHandler->keys->at(i)->getSDL_Key()){
					std::cout << "event key pressed" << std::endl;
					keyHandler->keys->at(i)->click(event.key.type);
				}
			}
		}

		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			this->keyHandler->builderCxt->resized();
		}
		
		// Quit Check
		if (event.type == SDL_EVENT_QUIT || event.key.key == SDLK_ESCAPE) {
			keyHandler->inputKeys->QUIT_KEY = true;
		}
		
		// Mouse
		if (event.type == SDL_EVENT_MOUSE_MOTION) {

			keyHandler->mouseKeys->MouseXPer = event.motion.x;
			keyHandler->mouseKeys->MouseYPer = event.motion.y;

			keyHandler->mouseKeys->MouseXrel = event.motion.xrel;
			keyHandler->mouseKeys->MouseYrel = event.motion.yrel;

		}
		
		// If Mouse is in game i.e. is being used
		if(SDL_GetWindowMouseGrab(this->window) && mouseHidden){
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					keyHandler->inputKeys->MOUSE_LEFT_CLICK = event.button.clicks;
					std::cout << "Button clicked" << std::endl;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					keyHandler->inputKeys->MOUSE_RIGHT_CLICK = event.button.clicks;
				}
			}
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					keyHandler->inputKeys->MOUSE_LEFT_CLICK = 0;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					keyHandler->inputKeys->MOUSE_RIGHT_CLICK = 0;
				}
			}
		}

		// Key Down
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
		
			if (event.key.key == keyHandler->keyBinds->UP_KEY)
			{
				keyHandler->inputKeys->UP_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->DOWN_KEY)
			{
				keyHandler->inputKeys->DOWN_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->LEFT_KEY)
			{
				keyHandler->inputKeys->LEFT_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->RIGHT_KEY)
			{
				keyHandler->inputKeys->RIGHT_KEY = true;
			}

			if (event.key.key == keyHandler->keyBinds->S_UP_KEY)
			{
				keyHandler->inputKeys->S_UP_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->S_DOWN_KEY)
			{
				keyHandler->inputKeys->S_DOWN_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->S_LEFT_KEY)
			{
				keyHandler->inputKeys->S_LEFT_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->S_RIGHT_KEY)
			{
				keyHandler->inputKeys->S_RIGHT_KEY = true;
			}

			if (event.key.key == keyHandler->keyBinds->ENTER_KEY) {
				keyHandler->inputKeys->ENTER_KEY = true;
			}
			



			// TOGGLE SWITCHES
			if (event.key.key == keyHandler->keyBinds->ADD_OBJECT_KEY && !keyHandler->inputKeys->ADD_OBJECT_KEY_T_Lock) {
				keyHandler->inputKeys->ADD_OBJECT_KEY_T = !keyHandler->inputKeys->ADD_OBJECT_KEY_T;
				keyHandler->inputKeys->ADD_OBJECT_KEY_T_Lock = true;
			}
			if (event.key.key == keyHandler->keyBinds->RESOURCE_TAB_KEY && !keyHandler->inputKeys->RESOURCE_TAB_KEY_T_Lock) {
				keyHandler->inputKeys->RESOURCE_TAB_KEY_T = !keyHandler->inputKeys->RESOURCE_TAB_KEY_T;
				keyHandler->inputKeys->RESOURCE_TAB_KEY_T_Lock = true;
			}
			if (event.key.key == keyHandler->keyBinds->STATUS_TAB_KEY && !keyHandler->inputKeys->STATUS_TAB_KEY_T_Lock) {
				keyHandler->inputKeys->STATUS_TAB_KEY_T = !keyHandler->inputKeys->STATUS_TAB_KEY_T;
				keyHandler->inputKeys->STATUS_TAB_KEY_T_Lock = true;
			}
			if (event.key.key == keyHandler->keyBinds->LIGHT_TAB_KEY && !keyHandler->inputKeys->LIGHT_TAB_KEY_T_Lock) {
				keyHandler->inputKeys->LIGHT_TAB_KEY_T = !keyHandler->inputKeys->LIGHT_TAB_KEY_T;
				keyHandler->inputKeys->LIGHT_TAB_KEY_T_Lock = true;
			}
			if (event.key.key == keyHandler->keyBinds->ALL_TAB_KEY && !keyHandler->inputKeys->ALL_TAB_KEY_T_Lock) {
				keyHandler->inputKeys->ALL_TAB_KEY_T = !keyHandler->inputKeys->ALL_TAB_KEY_T;
				keyHandler->inputKeys->ALL_TAB_KEY_T_Lock = true;
			}

			if (event.key.key == keyHandler->keyBinds->ACTION_KEY)
			{
				keyHandler->inputKeys->ACTION_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->SPRINT_KEY)
			{
				keyHandler->inputKeys->SPRINT_KEY = true;
			}
			if (event.key.key == keyHandler->keyBinds->SPACE_KEY) {
				keyHandler->inputKeys->SPACE_KEY = true;
			}
			

			// HELD KEYS
			if (event.key.key == keyHandler->keyBinds->CTRL_L_KEY) {
				keyHandler->heldKeys->lCrtl = true;
			}
			
			if (event.key.key == keyHandler->keyBinds->ALT_L_KEY) {
				keyHandler->heldKeys->lAlt = true;
			}
			if (event.key.key == keyHandler->keyBinds->CTRL_R_KEY) {
				keyHandler->heldKeys->rCrtl = true;
			}
			
			if (event.key.key == keyHandler->keyBinds->ALT_R_KEY) {
				keyHandler->heldKeys->rAlt = true;
			}

			// ACTION KEYS
			if (event.key.key == keyHandler->keyBinds->SPRINT_KEY) {
				keyHandler->inputKeys->SPRINT_KEY = true;
			}
			

		}
		
		// Key Up
		if (event.type == SDL_EVENT_KEY_UP)
		{
			if (event.key.key == keyHandler->keyBinds->FULLSCREEN)
			{
				keyHandler->inputKeys->FULLSCREEN = !keyHandler->inputKeys->FULLSCREEN;
			}
			
			if (event.key.key == keyHandler->keyBinds->CONSOL_KEY) {
				keyHandler->inputKeys->CONSOL_KEY = true;
			}

			if (event.key.key == keyHandler->keyBinds->S_UP_KEY)
			{
				keyHandler->inputKeys->S_UP_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->S_DOWN_KEY)
			{
				keyHandler->inputKeys->S_DOWN_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->S_LEFT_KEY)
			{
				keyHandler->inputKeys->S_LEFT_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->S_RIGHT_KEY)
			{
				keyHandler->inputKeys->S_RIGHT_KEY = false;
			}

			if (event.key.key == keyHandler->keyBinds->UP_KEY || event.key.key == keyHandler->keyBinds->S_UP_KEY)
			{
				keyHandler->inputKeys->UP_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->DOWN_KEY || event.key.key == keyHandler->keyBinds->S_DOWN_KEY)
			{
				keyHandler->inputKeys->DOWN_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->LEFT_KEY || event.key.key == keyHandler->keyBinds->S_LEFT_KEY)
			{
				keyHandler->inputKeys->LEFT_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->RIGHT_KEY || event.key.key == keyHandler->keyBinds->S_RIGHT_KEY)
			{
				keyHandler->inputKeys->RIGHT_KEY = false;
			}

			if (event.key.key == keyHandler->keyBinds->ACTION_KEY)
			{
				keyHandler->inputKeys->ACTION_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->SPRINT_KEY)
			{
				keyHandler->inputKeys->SPRINT_KEY = false;
			}
			if (event.key.key == keyHandler->keyBinds->SPACE_KEY) {
				keyHandler->inputKeys->SPACE_KEY = false;
			}

			if (event.key.key == keyHandler->keyBinds->ENTER_KEY) {
				keyHandler->inputKeys->ENTER_KEY = false;
			}

			// TOGGLE SWITCHES
			if (event.key.key == keyHandler->keyBinds->ADD_OBJECT_KEY ) {
				keyHandler->inputKeys->ADD_OBJECT_KEY_T_Lock = false;
			}
			if (event.key.key == keyHandler->keyBinds->RESOURCE_TAB_KEY ) {
				keyHandler->inputKeys->RESOURCE_TAB_KEY_T_Lock = false;
			}
			if (event.key.key == keyHandler->keyBinds->STATUS_TAB_KEY) {
				keyHandler->inputKeys->STATUS_TAB_KEY_T_Lock = false;
			}
			if (event.key.key == keyHandler->keyBinds->LIGHT_TAB_KEY) {
				keyHandler->inputKeys->LIGHT_TAB_KEY_T_Lock = false;
			}
			if (event.key.key == keyHandler->keyBinds->ALL_TAB_KEY) {
				keyHandler->inputKeys->ALL_TAB_KEY_T_Lock = false;
			}

			

			// HELD KEYS
			if (event.key.key == keyHandler->keyBinds->CTRL_L_KEY) {
				keyHandler->heldKeys->lCrtl = false;
			}
			
			if (event.key.key == keyHandler->keyBinds->ALT_L_KEY) {
				keyHandler->heldKeys->lAlt = false;
			}

			if (event.key.key == keyHandler->keyBinds->CTRL_R_KEY) {
				keyHandler->heldKeys->rCrtl = false;
			}

			if (event.key.key == keyHandler->keyBinds->ALT_R_KEY) {
				keyHandler->heldKeys->rAlt = false;
			}

			// ACTION KEYS
			if (event.key.key == keyHandler->keyBinds->SPRINT_KEY) {
				keyHandler->inputKeys->SPRINT_KEY = false;
			}
			
			
			if (event.key.key == keyHandler->keyBinds->HIDE_MOUSE_KEY && keyHandler->inputKeys->CTRL_L_KEY) {
				
				if (mouseHidden) {
					showMouse();
				}
				else
				{
					hideMouse();
				}
			}
			if (event.key.key == keyHandler->keyBinds->GRAB_MOUSE_KEY && keyHandler->inputKeys->CTRL_L_KEY) {
				SDL_SetWindowMouseGrab(window, !SDL_GetWindowMouseGrab(window));
			}

		}




	}

}


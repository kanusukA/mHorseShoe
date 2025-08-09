#pragma once

#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <mutex>
#include <SDL3/SDL.h>

struct MOUSEIN {

	int MouseXPer = 0;
	int MouseYPer = 0;

	double MouseXrel = 0.0f;
	double MouseYrel = 0.0f;

};


struct IKEYS {

	bool MouseLeft_KEY = false;
	bool MouseRight_KEY = false;
	bool MouseMiddle_KEY = false;

	bool UP_KEY = false;
	bool DOWN_KEY = false;
	bool LEFT_KEY = false;
	bool RIGHT_KEY = false;

	bool S_UP_KEY = false;
	bool S_DOWN_KEY = false;
	bool S_LEFT_KEY = false;
	bool S_RIGHT_KEY = false;

	bool SPRINT_KEY = false;
	bool QUIT_KEY = false;
	bool ACTION_KEY = false;

	bool SPACE_KEY = false;

	bool CONSOL_KEY = false;

	bool ALT_L_KEY = false;
	bool CTRL_L_KEY = false;

	// GUI CONTROL BINDS
	bool ADD_OBJECT_KEY_T = false;
	bool STATUS_TAB_KEY_T = false;
	bool RESOURCE_TAB_KEY_T = false;
	bool LIGHT_TAB_KEY_T = false;
	bool ALL_TAB_KEY_T = false;
	bool ADD_OBJECT_KEY_T_Lock = false;
	bool STATUS_TAB_KEY_T_Lock = false;
	bool RESOURCE_TAB_KEY_T_Lock = false;
	bool LIGHT_TAB_KEY_T_Lock = false;
	bool ALL_TAB_KEY_T_Lock = false;

	bool LEFT_CRTL_KEY = false;
	bool RIGHT_CRTL_KEY = false;
	bool LEFT_SHIFT_KEY = false;
	bool RIGHT_SHIFT_KEY = false;

	int MOUSE_LEFT_CLICK = 0;
	int MOUSE_RIGHT_CLICK = 0;

};


// Input Keys singleton
class InputHandler {

private:
	static InputHandler* pInstance_;
	static std::mutex mutex_;

	IKEYS* inputkeys = new IKEYS();
	MOUSEIN* mouseInputs = new MOUSEIN();

protected:
	InputHandler() {};
	~InputHandler() {};
public:

	// Class should not be clonable
	InputHandler(InputHandler& copy) = delete;

	// Class should not be assignable
	void operator=(const InputHandler&) = delete;

	static InputHandler* GetInstance();

	IKEYS* getInputKeys() { return inputkeys; }
	MOUSEIN* getMouseInput() { return mouseInputs; }

};


struct KEYBINDS {

	int UP_KEY = SDLK_UP;
	int DOWN_KEY = SDLK_DOWN;
	int LEFT_KEY = SDLK_LEFT;
	int RIGHT_KEY = SDLK_RIGHT;

	int S_UP_KEY = SDLK_W;
	int S_DOWN_KEY = SDLK_S;
	int S_LEFT_KEY = SDLK_D;
	int S_RIGHT_KEY = SDLK_A;

	int SPRINT_KEY = SDLK_LSHIFT;
	int QUIT_KEY = SDLK_ESCAPE;
	int ACTION_KEY = SDLK_E;
	int SPACE_KEY = SDLK_SPACE;

	int CONSOL_KEY = SDLK_C;

	int ALT_L_KEY = SDLK_LALT;
	int CTRL_L_KEY = SDLK_LCTRL;

	// GUI CONTROL BINDS
	int ADD_OBJECT_KEY = ACTION_KEY;
	int STATUS_TAB_KEY = SDLK_S;
	int RESOURCE_TAB_KEY = SDLK_R;
	int LIGHT_TAB_KEY = SDLK_L;
	int ALL_TAB_KEY = SDLK_H;

	// HELD CTRL
	int HIDE_MOUSE_KEY = SDLK_G;

	// DEBUG
	int GRAB_MOUSE_KEY = SDLK_M;

};

struct WINDOW_UTILS {
	int WINDOW_WIDTH = 0;
	int WINDOW_HEIGHT = 0;
};


class KeyHandler {
public:

	InputHandler* inputHandler = InputHandler::GetInstance();
	IKEYS* inputKeys = inputHandler->getInputKeys();
	MOUSEIN* mouseKeys = inputHandler->getMouseInput();
	KEYBINDS* keyBinds = new KEYBINDS();
	WINDOW_UTILS* winUtils = new WINDOW_UTILS();


	// IMPLEMENT IN FUTURE

	/*void setKeyBinds(KEYBINDS* kBinds);
	void setKeyBinds(int key, int SDLK);

	void saveBinds();
	void loadBinds();*/


};

#endif // !KEYHANDLER_H

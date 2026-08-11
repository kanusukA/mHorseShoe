
#include <SDL3/SDL.h>
#include <vector>
// THIS CLASS CONTAINS ALL DIFFERENT TYPE OF KEYS (OF VARIOUS COMBINATION ETC.)
#include <stdio.h>
#include <iostream>

typedef uint16_t MKeyCode;

class FeelKey {


public:

	// Prevents the button from sustaining true state for more than a single loop
	bool switchLock = false;

	std::vector<FeelKey*> heldKeys = std::vector<FeelKey*>();
	SDL_Keycode key;

	bool enabled = true;

	bool pressed = false;

	MKeyCode keyCode;

	FeelKey(uint32_t p_key, uint16_t mkeyCode , std::vector<FeelKey*> p_heldKeys = std::vector<FeelKey*>()) {
		key = p_key;
		heldKeys = p_heldKeys;
		keyCode = mkeyCode;
	}


	bool checkHit(SDL_Event& event) {

		if (switchLock)
		{
			pressed = false;
		}

		if (!heldKeys.empty())
		{
			for (const auto hKey : heldKeys)
			{
				if (!hKey->pressed)
				{
					return false;
				}
			}
		}
		if (event.key.key == key)
		{
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (switchLock)
				{
					pressed = false;
					return false;
				}
				else {
					switchLock = true;
					pressed = true;
					std::cout << key << " Pressed DOWN " << std::endl;
				}
				
			}
			else if (event.type == SDL_EVENT_KEY_UP) {
				switchLock = false;
				pressed = false;
				std::cout << key << " Pressed UP " << std::endl;
			}
				
		}

		return pressed;
	}


};

class FeelEvent {
public:

	bool enabled = true;

	bool eventState = false;
	uint32_t evtType;

	FeelEvent(uint32_t p_evtType) {
		evtType = p_evtType;
	}

	void checkEvent(SDL_Event& event) {
		if (event.type == evtType)
		{
			eventState = true;
		}
		else {
			eventState = false;
		}
	}


};

class FeelMouse {
public:
	float xRel;
	float yRel;

	void updateMousePos(SDL_Event& event) {
		xRel = 0.0f;
		yRel = 0.0f;

		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			xRel = event.motion.xrel / 10;
			yRel = event.motion.yrel / 10;
		}



	}

};

typedef FeelKey* MappedKey;
typedef FeelEvent* MappedEvent;

// MAPPED KEYS - KEYS THAT ARE MAPPED TO THEIR PURPOSE

// CUSTOM KEYCODES
constexpr MKeyCode MKEY_L_CTRL = 0;
constexpr MKeyCode MKEY_L_SHIFT = 1;
constexpr MKeyCode MKEY_FORWARD = 2;
constexpr MKeyCode MKEY_BACKWARD = 3;
constexpr MKeyCode MKEY_RIGHT = 4;
constexpr MKeyCode MKEY_LEFT = 5;
constexpr MKeyCode MKEY_UP = 6;
constexpr MKeyCode MKEY_DOWN = 7;
constexpr MKeyCode MKEY_WINDOW_GRAB = 8;

struct Keys
{

	MappedKey lCtrl;
	MappedKey lShift;

	MappedKey forwardKey;
	MappedKey backwardKey;
	MappedKey leftKey;
	MappedKey rightKey;
	MappedKey upKey;
	MappedKey downKey;

	MappedKey windowGrab;

};

struct Events {
	MappedEvent windowResize;
	MappedEvent quitApplication;
};


#include <SDL3/SDL.h>
#include <vector>
// THIS CLASS CONTAINS ALL DIFFERENT TYPE OF KEYS (OF VARIOUS COMBINATION ETC.)
#include <stdio.h>
#include <iostream>
class FeelKey {


public:

	// Prevents the button from sustaining true state for more than a single loop
	bool switchLock = false;

	std::vector<FeelKey*> heldKeys = std::vector<FeelKey*>();
	SDL_Keycode key;

	bool enabled = true;

	bool pressed = false;

	FeelKey(uint32_t p_key, std::vector<FeelKey*> p_heldKeys = std::vector<FeelKey*>()) {
		key = p_key;
		heldKeys = p_heldKeys;
	}

	void checkHit(SDL_Event& event) {

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
					return;
				}
			}
		}
		if (event.key.key == key)
		{
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (switchLock)
				{
					pressed = false;
					return;
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

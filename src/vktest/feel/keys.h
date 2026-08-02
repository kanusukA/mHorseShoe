
#include <SDL3/SDL.h>
#include <vector>
// THIS CLASS CONTAINS ALL DIFFERENT TYPE OF KEYS (OF VARIOUS COMBINATION ETC.)

class FeelKey {

public:

	std::vector<FeelKey*> heldKeys = std::vector<FeelKey*>();
	SDL_Keycode key;

	bool enabled = true;

	bool pressed = false;

	FeelKey(uint32_t p_key, std::vector<FeelKey*> p_heldKeys = std::vector<FeelKey*>()) {
		key = p_key;
		heldKeys = p_heldKeys;
	}

	void checkHit(SDL_Event& event) {
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
				pressed = true;
			}
			else if (event.type == SDL_EVENT_KEY_UP) {
				pressed = false;
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

	void updateMousePos() {
		// Get relative mouse pos from sdl3 
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

};

struct Events {
	MappedEvent windowResize;
	MappedEvent quitApplication;
};

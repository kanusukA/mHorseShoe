#include <feel/keys.h>


class Feel {
	std::vector<FeelKey> keys = {};
	std::vector<FeelEvent> events = {};
	
public:

	Keys mappedKeys = Keys();
	Events mappedEvents = Events();

	Feel() {
		setupDefaultEvents();
		setupDefaultKeys();
	}

	FeelKey* setKey(uint32_t keyCode, std::vector<FeelKey*> heldKeys = {}) {
		FeelKey newKey = FeelKey(keyCode, heldKeys);
		keys.emplace_back(newKey);
		return &keys.back();
	}

	FeelEvent* setEvent(SDL_EventType eventType) {
		FeelEvent newEvent = FeelEvent(eventType);
		events.emplace_back(newEvent);
		return &events.back();
	}

	void setupDefaultKeys();
	void setupDefaultEvents();

	void updateFeel() {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			for (auto& key : keys)
			{
				key.checkHit(event);
			}
			for (auto& evt : events)
			{
				evt.checkEvent(event);
			}
		}
		
	}

};
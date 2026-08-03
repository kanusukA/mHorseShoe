#include <feel/keys.h>

#include <glm/glm.hpp>

#include <mutex>

class Feel {
	std::vector<FeelKey*> keys = {};
	std::vector<FeelEvent*> events = {};
	
	static Feel* pinstance_;
	static std::mutex mutex_;
protected:

	Feel() {
		setupDefaultEvents();
		setupDefaultKeys();
	};
	~Feel() {};

public:

	Feel(Feel& copy) = delete;
	void operator=(const Feel&) = delete;

	static Feel* GetInstance();

	Keys mappedKeys = Keys();
	Events mappedEvents = Events();

	FeelMouse mouse = FeelMouse();

	FeelKey* setKey(uint32_t keyCode, std::vector<FeelKey*> heldKeys = {}) {
		FeelKey* newKey = new FeelKey(keyCode, heldKeys);
		keys.push_back(std::move(newKey));
		return keys.at(keys.size() -1 );
	}

	FeelEvent* setEvent(SDL_EventType eventType) {
		FeelEvent* newEvent = new FeelEvent(eventType);
		events.push_back(std::move(newEvent));
		return events.back();
	}

	void setupDefaultKeys();
	void setupDefaultEvents();

	void updateFeel() {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			mouse.updateMousePos(event);

			for (auto& key : keys)
			{
				key->checkHit(event);
			}
			for (auto& evt : events)
			{
				evt->checkEvent(event);
			}
		}
		
	}


	glm::vec3 getCameraKeyInput();
	glm::vec2 getCameraMouseInput();


};
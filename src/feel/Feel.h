#include <feel/keys.h>

#include <glm/glm.hpp>

#include <mutex>

// Extensions are a way to get pollEvents to other classes. This featuer is mainly required for Imgui but can be used in many other libraries.
// Extensions are executed first. Before any events and keys.
class FeelPollEventExtension {
public:
	virtual void pollEvent(SDL_Event& event) {};
	virtual void keyHit(MappedKey key) {};
};



class Feel {
	std::vector<FeelKey*> keys = {};
	std::vector<FeelEvent*> events = {};

	std::vector<FeelPollEventExtension*> extensions = {};
	
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

	FeelKey* setKey(uint32_t keyCode, MKeyCode mkeyCode, std::vector<FeelKey*> heldKeys = {}) {
		FeelKey* newKey = new FeelKey(keyCode, mkeyCode, heldKeys);
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
		
		mouse.resetValues();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			for (const auto& extension : extensions)
			{
				extension->pollEvent(event);
			}

			mouse.updateMousePos(event);
			

			for (auto& key : keys)
			{
				if (key->checkHit(event))
				{
					for (const auto& extension : extensions)
					{
						extension->keyHit(key);
					}
				}
			}
			for (auto& evt : events)
			{
				evt->checkEvent(event);
			}
		}
		
	}

	void addExtension(FeelPollEventExtension* p_extension) {
		extensions.push_back(p_extension);
	}


	glm::vec3 getCameraKeyInput();
	glm::vec2 getCameraMouseInput();


};
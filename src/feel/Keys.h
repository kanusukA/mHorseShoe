#pragma once

#include <GDHandler/GDContext.h>
#include <SDL3/SDL.h>

class Key {
protected:
	GDBuilderContext* builderCxt;

	SDL_Keycode SDL_Key;
	SDL_EventType eventType;

public:
	Key(GDBuilderContext* builderCxt_p, SDL_Keycode SDL_Key_p, SDL_EventType eventType) {
		builderCxt = builderCxt_p;
		SDL_Key = SDL_Key_p;
	}

	virtual void click() {}

	void setSDL_Key(SDL_Keycode SDL_Key_p) {
		SDL_Key = SDL_Key_p;
	}

	SDL_Keycode getSDL_Key() { return SDL_Key; }

	SDL_EventType getEventType() { return eventType; }

};


class FullScreenKey : public Key {
private:
	bool state;
public:
	FullScreenKey(GDBuilderContext* builderCxt_p, bool state_p = true) : Key(builderCxt_p,SDLK_F, SDL_EVENT_KEY_UP) {
		state = state_p;
	}

	void click() override {
		state = !state;
		std::cout << "Clicked";
		this->builderCxt->setFullScreen(state);
	}

};
#pragma once

#include <GDHandler/GDContext.h>
#include <SDL3/SDL.h>

class Key {
protected:
	GDBuilderContext* builderCxt;

	SDL_Keycode SDL_Key;


public:
	Key(GDBuilderContext* builderCxt_p, SDL_Keycode SDL_Key_p) {
		builderCxt = builderCxt_p;
		SDL_Key = SDL_Key_p;
		
	}

	virtual void click(SDL_EventType key_evnt) {
		
	}

	void setSDL_Key(SDL_Keycode SDL_Key_p) {
		SDL_Key = SDL_Key_p;
	}

	SDL_Keycode getSDL_Key() { return SDL_Key; }

};


class FullScreenKey : public Key {
private:
	bool state;
	bool lock = false;
public:
	FullScreenKey(GDBuilderContext* builderCxt_p, bool state_p = true) : Key(builderCxt_p,SDLK_F) {
		state = state_p;
	}

	void click(SDL_EventType key_evnt) override {
		if(!lock){
			state = !state;
			this->builderCxt->setFullScreen(state);
			lock = true;
		}
		if (key_evnt == SDL_EVENT_KEY_UP)
		{
			lock = false;
		}
	}

};

class CheckFunctionKey : public Key {
private:
	bool state;
	bool lock = false;
public:
	CheckFunctionKey(GDBuilderContext* builderCxt_p, bool state_p = true) : Key(builderCxt_p, SDLK_H) {
		state = state_p;
	}

	void click(SDL_EventType key_evnt) override {
		if (!lock) {
			state = !state;
			
			ToastComponent::GetInstance()->addMessage("Width : " + std::to_string(*builderCxt->getWindowSize()->width));
			ToastComponent::GetInstance()->addMessage("Height : " + std::to_string(*builderCxt->getWindowSize()->height));

			lock = true;
		}
		if (key_evnt == SDL_EVENT_KEY_UP)
		{
			lock = false;
		}
	}

};
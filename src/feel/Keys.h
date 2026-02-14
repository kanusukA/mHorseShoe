#pragma once

#include <GDHandler/GDContext.h>
#include <SDL3/SDL.h>

class HeldKeys {
public:
	bool lCrtl = false;
	bool rCrtl = false;

	bool lAlt = false;
	bool rAlt = false;

	bool lShift = false;
	bool rShift = false;
};

class Key {
protected:
	GDBuilderContext* builderCxt;
	HeldKeys* heldkeys;

	SDL_Keycode SDL_Key;


public:
	Key(GDBuilderContext* builderCxt_p, SDL_Keycode SDL_Key_p, HeldKeys* heldKeys_p) {
		heldkeys = heldKeys_p;
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


// KEYS _______________________________________________________________________________________

// SYSTEM KEYS ----------------------------------------------------------------------------

class FullScreenKey : public Key {
private:
	bool state;
	bool lock = false;
public:
	FullScreenKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, bool state_p = true) : Key(builderCxt_p,SDLK_F,heldKeys) {
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

class HideGuiKey : public Key {
private:
	bool state; 
	bool lock = false;
public:
	HideGuiKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, bool state_p = true) : Key(builderCxt_p, SDLK_H, heldKeys) {
		state = state_p;
	}

	void click(SDL_EventType key_evnt) override {
		if (!lock) {
			state = !state;
			this->builderCxt->setGuiVisibility(state);
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
	CheckFunctionKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, bool state_p = true) : Key(builderCxt_p, SDLK_H,heldKeys) {
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


class WinStateKey : public Key {
private:
	bool state;
	bool lock = false;
public:
	WinStateKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, bool state_p = true) : Key(builderCxt_p, SDLK_G,heldKeys) {
		state = state_p;
	}

	void click(SDL_EventType key_evnt) override {
		if (!lock) {
			state = !state;
			if (state && heldkeys->lCrtl)
			{
				builderCxt->changeWindowState(GDSun::IN_GAME);
			}
			else {
				builderCxt->changeWindowState(GDSun::GUI);
			}
			

			lock = true;
		}
		if (key_evnt == SDL_EVENT_KEY_UP)
		{
			lock = false;
		}
	}
};


// PLAYER MOVEMENT KEYS -----------------------------------------------------------------

class CamForwardKey : public Key {
private:
	PlayerInput* input;

public:
	CamForwardKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, PlayerInput* input_p) : Key(builderCxt_p, SDLK_W,heldKeys) {
		input = input_p;
	}
	void click(SDL_EventType key_evnt) override {
		if (key_evnt == SDL_EVENT_KEY_DOWN)
		{
			input->forward = true;
		}
		else {
			input->forward = false;
		}
	}

};

class CamBackwardKey : public Key {
private:
	PlayerInput* input;

public:
	CamBackwardKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, PlayerInput* input_p) : Key(builderCxt_p, SDLK_S,heldKeys) {
		input = input_p;
	}
	void click(SDL_EventType key_evnt) override {
		if (key_evnt == SDL_EVENT_KEY_DOWN)
		{
			input->backward = true;
		}
		else {
			input->backward = false;
		}
	}

};

class CamLeftKey : public Key {
private:
	PlayerInput* input;

public:
	CamLeftKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, PlayerInput* input_p) : Key(builderCxt_p, SDLK_A,heldKeys) {
		input = input_p;
	}
	void click(SDL_EventType key_evnt) override {
		if (key_evnt == SDL_EVENT_KEY_DOWN)
		{
			input->left = true;
		}
		else {
			input->left = false;
		}
	}

};

class CamRightKey : public Key {
private:
	PlayerInput* input;

public:
	CamRightKey(GDBuilderContext* builderCxt_p, HeldKeys* heldKeys, PlayerInput* input_p) : Key(builderCxt_p, SDLK_D,heldKeys) {
		input = input_p;
	}
	void click(SDL_EventType key_evnt) override {
		if (key_evnt == SDL_EVENT_KEY_DOWN)
		{
			input->right = true;
		}
		else {
			input->right = false;
		}
	}

};
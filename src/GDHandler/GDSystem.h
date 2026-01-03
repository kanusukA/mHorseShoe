#pragma once

#include <GDHandler/stuff/Stuff.h>

struct SunWindowSize
{
	int* height = new int(0);
	int* width = new int(0);
};

class GDSystem {

protected:
	SDL_Window* window;

	SunWindowSize* windowSize = new SunWindowSize();

	void setWindowSize();
	

public:
	GDSystem(Monster* monster) {
		monster->InitMonster();

		window = monster->sdlWindow;

	}

	SunWindowSize* getWindowSize() {
		return windowSize;
	}

	void setFullScreen(bool mode) {
		SDL_SetWindowFullscreen(window, mode);
		
	}

	void resized();
	


};
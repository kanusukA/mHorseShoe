#pragma once

#include <GDHandler/stuff/Stuff.h>

#include <GDHandler/GDNotifier.h>

struct SunWindowSize
{
	int* height = new int(0);
	int* width = new int(0);
};

namespace GDSun {
	enum WINDOW_STATE {
		IN_GAME,
		GUI,
		PAUSE
	};
}

class GDSystem : public GDNotifier {

protected:
	SDL_Window* window;

	SunWindowSize* windowSize = new SunWindowSize();

	void setWindowSize();

	GDSun::WINDOW_STATE winState = GDSun::GUI;
	bool GuiVisibility = true;
	

public:
	GDSystem(Monster* monster) : GDNotifier() {
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
	
	void changeWindowState(GDSun::WINDOW_STATE state) {
		winState = state;
		if (winState == GDSun::IN_GAME)
		{
			ShowCursor(false);
			SDL_SetWindowRelativeMouseMode(window, true);
		}
		else {
			ShowCursor(true);
			SDL_SetWindowRelativeMouseMode(window, false);
		}
	}

	GDSun::WINDOW_STATE getWindowState() {
		return winState;
	}

	void setGuiVisibility(bool visible) {
		GuiVisibility = visible;
	}

	bool getGuiVisibility() {
		return GuiVisibility;
	}

};
#include <feel/feel.h>

void Feel::setupDefaultKeys() {

	mappedKeys.lCtrl = setKey(SDLK_LCTRL);
	mappedKeys.lShift = setKey(SDLK_LSHIFT);

	mappedKeys.forwardKey = setKey(SDLK_W);
	mappedKeys.backwardKey = setKey(SDLK_S);
	mappedKeys.upKey = setKey(SDLK_W, { mappedKeys.lCtrl });
	mappedKeys.downKey = setKey(SDLK_S, { mappedKeys.lShift });

	mappedKeys.leftKey = setKey(SDLK_A);
	mappedKeys.rightKey = setKey(SDLK_D);

}

void Feel::setupDefaultEvents() {

	mappedEvents.windowResize = setEvent(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED);
	mappedEvents.quitApplication = setEvent(SDL_EVENT_QUIT);

}
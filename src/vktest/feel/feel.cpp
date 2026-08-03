#include <feel/feel.h>


Feel* Feel::pinstance_{ nullptr };
std::mutex Feel::mutex_;

Feel* Feel::GetInstance(){
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr)
	{
		pinstance_ = new Feel();
	}
	return pinstance_;
}


void Feel::setupDefaultKeys() {

	mappedKeys.lCtrl = setKey(SDLK_LCTRL);
	mappedKeys.lShift = setKey(SDLK_LSHIFT);

	mappedKeys.forwardKey = setKey(SDLK_W);
	mappedKeys.backwardKey = setKey(SDLK_S);
	mappedKeys.upKey = setKey(SDLK_W, { mappedKeys.lCtrl });
	mappedKeys.downKey = setKey(SDLK_S, { mappedKeys.lShift });

	mappedKeys.leftKey = setKey(SDLK_A);
	mappedKeys.rightKey = setKey(SDLK_D);

	mappedKeys.windowGrab = setKey(SDLK_G);
	mappedKeys.windowGrab->switchLock = true;

}

void Feel::setupDefaultEvents() {

	mappedEvents.windowResize = setEvent(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED);
	mappedEvents.quitApplication = setEvent(SDL_EVENT_QUIT);

}

glm::vec3 Feel::getCameraKeyInput()
{
	glm::vec3 updatePos = glm::vec3(0.0f);
	if (mappedKeys.upKey->pressed)
	{
		updatePos.y = 1.0f;
	}else if (mappedKeys.forwardKey->pressed)
	{
		updatePos.x = 1.0f;
	}
	if (mappedKeys.downKey->pressed)
	{
		updatePos.y = -1.0f;

	}else if (mappedKeys.backwardKey->pressed)
	{
		updatePos.x = -1.0f;
	}
	
	
	if (mappedKeys.rightKey->pressed)
	{
		updatePos.z = 1.0f;
	}
	if (mappedKeys.leftKey->pressed)
	{
		updatePos.z = -1.0f;
	}

	return updatePos;
}

glm::vec2 Feel::getCameraMouseInput()
{
	return glm::vec2(mouse.xRel, mouse.yRel);
}

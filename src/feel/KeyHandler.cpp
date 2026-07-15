#include "KeyHandler.h"

InputHandler* InputHandler::pInstance_{ nullptr };
std::mutex InputHandler::mutex_;

InputHandler* InputHandler::GetInstance()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (pInstance_ == nullptr) {
		pInstance_ = new InputHandler();
	}
	return pInstance_;

}

KeyHandler::KeyHandler(/*GDBuilderContext* builderCxt_p*/)
{
//	builderCxt = builderCxt_p;

	/*FullScreenKey* fullScreenKey = new FullScreenKey(builderCxt_p,heldKeys);
	HideGuiKey* hideGuiKey = new HideGuiKey(builderCxt_p, heldKeys);
	CheckFunctionKey* checkFuncKey = new CheckFunctionKey(builderCxt_p, heldKeys);
	WinStateKey* winStateKey = new WinStateKey(builderCxt_p, heldKeys);

	keys->push_back(winStateKey);
	keys->push_back(fullScreenKey);
	keys->push_back(checkFuncKey);
	keys->push_back(hideGuiKey);
	*/
}
/*
void KeyHandler::setPlayerInput(PlayerInput* pInput_p)
{
	CamForwardKey* camForwardKey = new CamForwardKey(builderCxt, heldKeys, pInput_p);
	CamBackwardKey* camBackwardKey = new CamBackwardKey(builderCxt, heldKeys, pInput_p);
	CamLeftKey* camLeftKey = new CamLeftKey(builderCxt, heldKeys, pInput_p);
	CamRightKey* camRightKey = new CamRightKey(builderCxt, heldKeys, pInput_p);

	keys->push_back(camForwardKey);
	keys->push_back(camBackwardKey);
	keys->push_back(camLeftKey);
	keys->push_back(camRightKey);

	pInput_p->MouseXrel = &mouseKeys->MouseXrel;
	pInput_p->MouseYrel = &mouseKeys->MouseYrel;


}*/

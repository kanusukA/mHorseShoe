#pragma once


#ifndef GDHANDLER_H
#define GDHANDLER_H

#include <GDHandler/StartPlatinum.h>
#include <feel/Feel.h>

// THIS CLASS IS CREATED TO MANAGE PRE-MADE FUNCTIONS THAT CAN SCOPE (ALMOST) EVERY OTHER COMPONENT IN THIS SYSTEM
// USE OF THIS CLASS IS NOT ADIVISED IF THE SCOPE REQUIRES ONLY A SINGLE COMPONENT TO BE ALTERED.
class GDHandler : public CaseHandler
{

public:

	Feel* feel;

	PlayerSubject* playerSubject = nullptr;

//	Gui* gui;

	GDHandler(ResourceHandler* resourceHan_p, Monster* renderer, Feel* feel_p) : CaseHandler(renderer) {

		// Init resources
//		ResourceHandler::GetInstance();

		//PlayerInput* pInput = new PlayerInput();
		//playerSubject = new PlayerSubject(monster->getCameraScnNode(), monster->getCamera(), pInput);
		

		feel = feel_p;
		KeyHandler* keyHandler = new KeyHandler(this);
		//keyHandler->setPlayerInput(pInput);

		feel->initFeel(renderer->sdlWindow, keyHandler);

		//gui = new Gui(this, ResourceHandler::GetInstance(), RSUS::GetInstance());

		//gui->initGui(this->monster->imguiOverlay);

		//loadSavedCase();


	};

	// LOADS THE DEFAULT CASE FROM THE CASE_CONFIG.YML FILE AND SELECTS IT IN THE MODEL_COMPONENT!
	void loadSavedCase();

	// NOTIFIER FUNCTIONS MUST BE REMOVED WHEN CASE HANDLER IS OVERHAULED!
	virtual void notifyLoadCase() override;
	virtual void notifySaveCase() override;

	void update(float deltatime) override;


};



#endif // !GDHANDLER_H



#pragma once


#ifndef GUI_H
#define GUI_H

#include <Gui/GuiComponents.h>


// Components

struct GUI_WINDOW_POS {
	ImVec2 TOP_LEFT;
	ImVec2 TOP_RIGHT;
	ImVec2 TOP_CENTER;

	ImVec2 RIGHT;
	ImVec2 LEFT;
	ImVec2 CENTER;

	ImVec2 BOTTOM_LEFT;
	ImVec2 BOTTOM_RIGHT;
	ImVec2 BOTTOM_CENTER;

};


// Initalize from the Ogre::ApplicationContext -> initImGuiOverlay()
// Add imGui input listner to context input listner.
class Gui : public GuiFramework
{
private:


	Ogre::ImGuiOverlay* imOverlay = nullptr;

	GUI_WINDOW_POS* guiWPos = new GUI_WINDOW_POS();

	void addViewComponent(ViewComponent* viewComponent) override {
		viewComponent->setFramework(this);
		Views.push_back(viewComponent);
	}

	void addModelComponent(ModelComponent* modelComponent) override {
		modelComponent->setFramework(this);
		modelComponent->setSource(this);
		modelComponent->init();
		Models.push_back(modelComponent);
	}

	Gui();

public:

	Gui(CaseHandler* casehan, StuffHandler* stuffhan, ResourceHandler* resourcehan, RSUS* rsus) : 
		GuiFramework(casehan, stuffhan,resourcehan, rsus) 
	{
			
	}

	//void setPlayerObserver(PlayerObserver* pObserver);
	ImGuiViewport* viewport;

	// INIT
	void initGui(Ogre::ImGuiOverlay* overlay);

	// NEW FRAMEWORK
	// Add ViewComponents / ModelComponents in this function
	// Components not init here will not render and will not enter the GuiFramework
	// View Component that require Model Component must have the specified Model Component initialized before the View Component.
	void initGuiComponents();
	void updateGuiComponents();

	void setWindowGrabPoints(int WIDTH, int HEIGHT);
	void updateGui(); // updates visibility of gui tabs
	void shutdown();


};

#endif // !GUI_H









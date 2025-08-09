#pragma once


#ifndef GUI_H
#define GUI_H

#include <Gui/mediator/GuiMediator.h>
#include <GDHandler/ResourceHandler.h>
#include <feel/playerMovement.h>

#include <imgui_stdlib.h>


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
class Gui
{
private:

	Ogre::ImGuiOverlay* imOverlay = nullptr;

	GUI_WINDOW_POS* guiWPos = new GUI_WINDOW_POS();

	ImGuiViewport* viewport;

	GuiComponent* guiComponent;

	ResourceHandler* resourceHandler = ResourceHandler::GetInstance();

	PlayerObserver* playerObserver = nullptr;

	// Add Variable
	int renderMeshesPosition = 0;
	int colliderMeshesPosition = 0;
	Ogre::ResourceHandle hdl;
	Ogre::ResourceHandle grassHdl;


	int* fps = new int(0);

public:

	void setPlayerObserver(PlayerObserver* pObserver);

	// INIT
	void initGui(Ogre::ImGuiOverlay* overlay, GuiComponent* component);
	void setWindowGrabPoints(int WIDTH, int HEIGHT);
	void updateGui(); // updates visibility of gui tabs
	void shutdown();

	// INTERNAL
	void _resourceTab();
	void _statusTab();
	void _heightMapTab();
	void _addTab();
	void _debugTab();
	void _objectsPanel();
	void _lightTab();
	void _objectTab();
	void _HUD();
	void _diegnos();
	void _RSUSTab();


	void refreshResources() {
		ResourceHandler::GetInstance()->getAllResources();
	}

	// EXTERNAL
	void GuiInput(); // reads keyinputs for guitabs update
	void setDiagnosticStats(Diegnostics* die) { guiComponent->setDiegnostics(die); }



};

#endif // !GUI_H









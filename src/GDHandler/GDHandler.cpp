#include "GDHandler.h"

// Add newly made player Subject and observers
// Get this shit working

GDHandler::GDHandler(Monster* renderer, Kint* physics) {
	
	monster = renderer;

	// initalize monster
	monster->InitMonster();
	std::cout << "Monster window - SDL3 created!" << std::endl;
	// FEELS
	initFeel(monster->sdlWindow);

	this->stuffHandler = new StuffHandler(monster,physics);

	SceneHandler::GetInstance()->setStuffHandler(stuffHandler);

	gui = new Gui(SceneHandler::GetInstance(), this->stuffHandler,ResourceHandler::GetInstance(),RSUS::GetInstance());

	if (this->playerSubject == nullptr) {
		this->playerSubject = new PlayerSubject(monster->getPlayerSceneNode(), monster->getCamera());
	}

	PlayerObserver* playerObserver = new PlayerObserver(this->playerSubject);
	// Show Gui Player info
	gui->setPlayerObserver(playerObserver);

}

void GDHandler::initGui(Ogre::ImGuiOverlay* overlay) {
	gui->initGui(overlay);
}

// Fetches Resources 
void GDHandler::preSetup()
{
	// Resource 
	
	SceneObject obj = SceneObject();
	obj.name = "Cube";
	
	

}


void GDHandler::updateGDHandler(float deltaTime)
{
	// INPUT
	updateInput(deltaTime);
	// PLAYER UPDATE
	_updatePlayerPrameters(deltaTime);


	// UPDATE GUI
	gui->updateGui();
	// UPDATE PHYSICS AND RENDERING
	this->stuffHandler->update(deltaTime);

}

void GDHandler::_updatePlayerPrameters(float deltaTime)
{
	// PLAYER MOVEMENT
	// Updates the player Scene node set in the monster player Node
	// Updates the playerSubject which in turn updates all the assigned Observers
	if (playerSubject != nullptr && mouseHidden) {
		updateRotation(deltaTime, playerSubject);
		updatePosition(deltaTime, playerSubject);
	}

}



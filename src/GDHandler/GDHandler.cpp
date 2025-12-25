#include "GDHandler.h"

// Add newly made player Subject and observers
// Get this shit working




void GDHandler::update(float deltatime)
{
	feel->updateInput(deltatime);

	gui->updateGui();

	kint->updatePhysics(deltatime);

	// updates Monster and Kint
	stuffHandler->update(deltatime);

	monster->updateMonster();




}


//void GDHandler::_updatePlayerPrameters(float deltaTime)
//{
//	// PLAYER MOVEMENT
//	// Updates the player Scene node set in the monster player Node
//	// Updates the playerSubject which in turn updates all the assigned Observers
//	if (playerSubject != nullptr && mouseHidden) {
//		updateRotation(deltaTime, playerSubject);
//		updatePosition(deltaTime, playerSubject);
//	}
//
//}



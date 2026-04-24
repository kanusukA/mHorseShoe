#include "GDHandler.h"

void GDHandler::notifyLoadCase() {
	std::cout << "Load case notified" << std::endl;
	
	// LOAD GUI FRAMEWORK
	gui->updateLoadCase();
}

void GDHandler::notifySaveCase() {
	std::cout << "Save case notified" << std::endl;
}

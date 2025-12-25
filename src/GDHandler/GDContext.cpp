#include "GDContext.h"

void GDBuilderContext::startEngine()
{
	// Start Ogre Rendering engine and inits SdlWindow
	monster->InitMonster();

	// Starts Sdl Window for key, mouse and other inputs
	feel->initFeel(monster->sdlWindow);

	// Start PhysX
	kint->InitPhysics();


}

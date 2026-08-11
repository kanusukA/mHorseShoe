
//#include <GDHandler/GDHandler.h>
#include <monster/Monster.h>

#include <timer/glock.h>
#include <cons.h>




// Rendering
// Physics
// GUI


int main() {

	std::cout << "Start : " << std::endl;

//	OgreBites::ApplicationContext ctx(INSTANCE_NAME);
//	ctx.initApp();

	std::cout << "Ogre started : " << std::endl;

//	ResourceHandler::GetInstance();

	//ResourceHandler::GetInstance()->/*addOgreRenderMeshResourceLocation*/();

	// Ogre AND ImGui
	std::cout << "Setting up ImGui" << std::endl;
//	Ogre::ImGuiOverlay* imOverlay = ctx.initialiseImGui(); // initalizes imgui before InitMonster! else will pop errors in renderOneFrame in Loop.
//	ctx.addInputListener(ctx.getImGuiInputListener());

	// Ogre Overlay INIT
	std::cout << "Initializing monster" << std::endl;
//	Monster* monster = new Monster(ctx.getRoot(),ctx.getRenderWindow(), ctx.getOverlaySystem(),imOverlay);
	//setup materials to ogre
	Monster* monster = new Monster();

	monster->InitMonster();

	std::cout << "Initializing kint" << std::endl;
	//Kint* kint = new Kint();
	
//	Ogre::Root* oRoot = monster->oRoot;

//	monster->addMainDirectionalLight(MAIN_DIRECTIONAL_LIGHT_NAME, Ogre::Vector3(0, -0.6, 0.4), 2);


	std::cout << "Setting up Kint" << std::endl;
	// Physics INIT
	/*kint = new Kint();
	kint->InitPhysics();*/
	
	

//	feel->initFeel(monster->sdlWindow,
	//GDHANDLER
	
	//GDHandler* gdHandler = new GDHandler(ResourceHandler::GetInstance(),monster,feel); // TODO integrate Feel with GDHandler itself

	//monster->setGrid();
	

	// TODO SETUP BETTER STARTUP
	//std::cout << "Setting up Skybox" << std::endl;
	//monster->setSkyBox();
	//monster->setGrid();

	//monster->_createGrassBlade(0.3, 1);

	Glock* glock = new Glock();

	// MAIN LOOP
	
	while (!Feel::GetInstance()->mappedEvents.quitApplication->eventState) {

		glock->setStartTime();

		Feel::GetInstance()->updateFeel();
		

		monster->updateMonster(Feel::GetInstance()->getCameraKeyInput(), Feel::GetInstance()->getCameraMouseInput(),glock->deltaTime);

		glock->setEndTime();
		
	}

	std::cout << "loop ended : " << std::endl;

//	ResourceHandler::GetInstance()->saveResources();

	// Shutdown
	//kint->Shutdown();
	monster->Shutdown();

	//ResourceHandler::GetInstance()->shutdown();

	std::cout << "HEllOS";

	
}

     


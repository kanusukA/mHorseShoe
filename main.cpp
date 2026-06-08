
#include <GDHandler/GDHandler.h>
#include <timer/glock.h>



// Rendering
// Physics
// GUI

// 16 millisec in each frame for 60FPS
const double MS_PER_FRAME = 16;

int main() {

	std::cout << "Start : " << std::endl;

	OgreBites::ApplicationContext ctx(INSTANCE_NAME);
	ctx.initApp();

	std::cout << "Ogre started : " << std::endl;

	ResourceHandler::GetInstance();

	//ResourceHandler::GetInstance()->addOgreRenderMeshResourceLocation();

	// Ogre AND ImGui
	std::cout << "Setting up ImGui" << std::endl;

	// Setup fonts for IMGUI Overlay in Monster
	// This process cannot be done in GUI itself as it requires Imgui to be uninitalized. This was changed in Imgui version 1.92+ but Ogre uses 1.91.9b currently.
	// TODO CHANGE THIS METHOD TO RUN AT GUI VIEW COMPONENT WHEN IMGUI VERSION IS UPDATED TO 1.92+
	Monster* monster = new Monster();
	monster->setupFonts();

	Ogre::ImGuiOverlay* imOverlay = ctx.initialiseImGui(); // initalizes imgui before InitMonster! else will pop errors in renderOneFrame in Loop.
	ctx.addInputListener(ctx.getImGuiInputListener());

	// Ogre Overlay INIT
	monster->InitMonster(ctx.getRoot(), ctx.getRenderWindow(), ctx.getOverlaySystem(), imOverlay);
	
	//setup materials to ogre
	

	std::cout << "Initializing kint" << std::endl;
	//Kint* kint = new Kint();
	
	Ogre::Root* oRoot = monster->oRoot;

	// SHADOM V5 DOES NOT WORK WITH CONVENTIONAL MAIN DIRECTIONAL LIGHT AS IT DOES NOT HAVE A FIXED POSITION IN SPACE AND CAUSES ARTIFACTS!
	// AS AN ALTERNATIVE A DIRECTION IS PASSED TO THE SHADER WHICH IS THEN USED TO CALCULATE MAIN DIRECTIONAL LIGHT.
	//monster->addMainDirectionalLight(MAIN_DIRECTIONAL_LIGHT_NAME, Ogre::Vector3(0, -0.6, 0.4), 2);


	std::cout << "Setting up Kint" << std::endl;
	// Physics INIT
	/*kint = new Kint();
	kint->InitPhysics();*/

	Feel* feel = new Feel();

	//GDHANDLER
	
	GDHandler* gdHandler = new GDHandler(ResourceHandler::GetInstance(),monster,feel); // TODO integrate Feel with GDHandler itself

	//monster->setGrid();
	

	// TODO SETUP BETTER STARTUP
	//std::cout << "Setting up Skybox" << std::endl;
	//monster->setSkyBox();
	//monster->setGrid();

	//monster->_createGrassBlade(0.3, 1);

	// MAIN LOOP
	double startTime;
	double renderTime;
	double elapsed;
	double deltaTime;

	double lastTime = getCurrentTime();

	std::cout << "loop started : " << std::endl;
	
	while (!InputHandler::GetInstance()->getInputKeys()->QUIT_KEY) {

		startTime = getCurrentTime();
		elapsed = startTime - lastTime;
		deltaTime = elapsed / 100;

		// Input / GUI Update
		gdHandler->update(deltaTime);

		
		renderTime = getCurrentTime();


		lastTime = startTime;

		if (startTime + MS_PER_FRAME > getCurrentTime()) {
			Sleep(startTime + MS_PER_FRAME - getCurrentTime());
		}
	

	}

	std::cout << "loop ended : " << std::endl;



	// Shutdown
	//kint->Shutdown();
	monster->Shutdown();

	ResourceHandler::GetInstance()->shutdown(); // SAVES LOAD PATHS AS WELL!

	std::cout << "HEllOS";

	
}

     


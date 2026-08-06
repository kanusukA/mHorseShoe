
//#include <GDHandler/GDHandler.h>
#include "Monster.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_oldnames.h"
//#include <timer/glock.h>
//#include <cons.h>



#include <glock.h>

// Rendering
// Physics
// GUI

// 16 millisec in each frame for 60FPS
const double MS_PER_FRAME = 16;

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
	//std::filesystem::path mpath = std::filesystem::path("D:/source/repos/mHorseShoeVCmake/mHorseShoe/src/vktest/shaders/orangeBox.obj");
	std::filesystem::path mpath = std::filesystem::path("../../../vktest/shaders/orangeBox.obj");
	auto apath = std::filesystem::absolute(mpath);

	WCHAR path[MAX_PATH];

	GetModuleFileNameW(NULL, path, MAX_PATH);

	char strPath[MAX_PATH];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, path, -1, strPath, MAX_PATH, &DefChar, NULL);
	std::filesystem::path srcPath = strPath;
	std::cout << "Current Path" << srcPath.remove_filename();

	if (std::filesystem::exists(apath))
	{
		throw std::runtime_error("File does not exist!");
	}

	auto mesh =	monster->loadMeshObj(apath);

	monster->presentMesh(mesh);
	
//	Ogre::Root* oRoot = monster->oRoot;

//	monster->addMainDirectionalLight(MAIN_DIRECTIONAL_LIGHT_NAME, Ogre::Vector3(0, -0.6, 0.4), 2);

	
	Glock glock = Glock();
	

	std::cout << "Setting up Kint" << std::endl;
	// Physics INIT
	/*kint = new Kint();
	kint->InitPhysics();*/

	//Feel* feel = new Feel();


//	feel->initFeel(monster->sdlWindow,
	//GDHANDLER
	
	//GDHandler* gdHandler = new GDHandler(ResourceHandler::GetInstance(),monster,feel); // TODO integrate Feel with GDHandler itself

	//monster->setGrid();
	

	// TODO SETUP BETTER STARTUP
	//std::cout << "Setting up Skybox" << std::endl;
	//monster->setSkyBox();
	//monster->setGrid();

	//monster->_createGrassBlade(0.3, 1);

	// MAIN LOOP

	Feel::GetInstance()->addExtension(monster);


//	double lastTime = getCurrentTime();

	std::cout << "loop started : " << std::endl;

	bool running = true;
	//SDL_Event event;
	
	while (/*!InputHandler::GetInstance()->getInputKeys()->QUIT_KEY*/ running) {


		glock.setStartTime();

		Feel::GetInstance()->updateFeel();

		if (Feel::GetInstance()->mappedEvents.windowResize->eventState)
		{
			monster->framebufferResized = true;
		}
		if (Feel::GetInstance()->mappedEvents.quitApplication->eventState)
		{
			running = false;
		}
		/*if (Feel::GetInstance()->mappedKeys.windowGrab->pressed)
		{
			std::cout << "HIT" << std::endl;
			monster->grabMouse(!monster->windowGrabbed);
		}*/

		/*while (SDL_PollEvent(&event)) {

			if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
			{
				monster->framebufferResized = true;
			}
		
			if (event.type == SDL_EVENT_QUIT ) {
			
				running = false;
			}
		}*/

		monster->updateMonster(Feel::GetInstance()->getCameraKeyInput(), Feel::GetInstance()->getCameraMouseInput(), glock.deltaTime);

//		startTime = getCurrentTime();
//		elapsed = startTime - lastTime;
//		deltaTime = elapsed / 100;

		// Input / GUI Update
		//gdHandler->update(deltaTime);

		
//		renderTime = getCurrentTime();


//		lastTime = startTime;

/*		if (startTime + MS_PER_FRAME > getCurrentTime()) {
			Sleep(startTime + MS_PER_FRAME - getCurrentTime());
		}
*/	

		glock.setEndTime();

		if (glock.getDelta() < MS_PER_FRAME)
		{
			glock.setSleep(MS_PER_FRAME - glock.deltaTime);
		}

	}

	std::cout << "loop ended : " << std::endl;

//	ResourceHandler::GetInstance()->saveResources();

	// Shutdown
	//kint->Shutdown();
	monster->Shutdown();

	//ResourceHandler::GetInstance()->shutdown();

	std::cout << "HEllOS";

	
}

     


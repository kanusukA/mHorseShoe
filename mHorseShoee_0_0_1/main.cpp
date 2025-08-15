
#include <GDHandler/GDHandler.h>

#include <cons.h>
#include <timer/glock.h>


// Rendering
// Physics
// GUI


// 16 millisec in each frame for 60FPS
const double MS_PER_FRAME = 16;

int main() {

	std::cout << "Start : " << std::endl;

	Diegnostics* diegnos = new Diegnostics();

	OgreBites::ApplicationContext ctx(INSTANCE_NAME);
	ctx.initApp();

	std::cout << "Ogre started : " << std::endl;

	// Ogre Overlay INIT
	std::cout << "Initializing monster" << std::endl;
	Monster* monster = new Monster(ctx.getRoot(),ctx.getRenderWindow(), ctx.getOverlaySystem());

	std::cout << "Initializing kint" << std::endl;
	Kint* kint = new Kint();
	
	Ogre::Root* oRoot = monster->oRoot;

	// Ogre AND ImGui
	std::cout << "Setting up ImGui" << std::endl;
	GuiComponent* guiComponent = new GuiComponent();
	Ogre::ImGuiOverlay* imOverlay = ctx.initialiseImGui(); // initalizes imgui before InitMonster! else will pop errors in renderOneFrame in Loop.
	ctx.addInputListener(ctx.getImGuiInputListener());

	monster->addMainDirectionalLight(MAIN_DIRECTIONAL_LIGHT_NAME, Ogre::Vector3(0, -0.9, -0.5), 1);

	std::cout << "Setting up Kint" << std::endl;
	// Physics INIT
	kint = new Kint();
	kint->InitPhysics();

	//GDHANDLER


	
	GDHandler gdhandler = GDHandler(monster,kint);
	gdhandler.preSetup();
	std::cout << "GDHandler setup!" << std::endl;
	gdhandler.initGui(imOverlay, guiComponent); // initaliz Gui Seperately from monster as it conflicts with Stuff
	std::cout << "Gui Initialized !" << std::endl;
	gdhandler.addPlayerNode();
	std::cout << "Player initialized!" << std::endl;
	

	guiComponent->setDiegnostics(diegnos);

	std::cout << "Setting up Skybox" << std::endl;
	monster->setSkyBox();
	monster->setGrid();

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

		// FIX IN FUTURE IF GUI AND THE RENDERING IS OUT OF SYNC!!
		// SEPERATE THE GUI UPDATE FUNCTION IN GDHANDLER AND UPDATE NEAR RENDERING

		// Input / GUI Update
		gdhandler.updateGDHandler(deltaTime);

		
		// Physics Update
		kint->updatePhysics(deltaTime);

		renderTime = getCurrentTime();

		oRoot->renderOneFrame();
		monster->updateMonster();

		*diegnos->renderFrameTime = getCurrentTime() - renderTime;

		*diegnos->fps = 1000/(getCurrentTime() - startTime);
		*diegnos->TotalFrameTime = getCurrentTime() - startTime;

		lastTime = startTime;

		if (startTime + MS_PER_FRAME > getCurrentTime()) {
			Sleep(startTime + MS_PER_FRAME - getCurrentTime());
		}
	

	}

	std::cout << "loop ended : " << std::endl;

	// Shutdown
	kint->Shutdown();
	monster->Shutdown();

	std::cout << "HEllOS";

	
}

     


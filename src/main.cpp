
//#include <GDHandler/GDHandler.h>
#include <Gui/Gui.h>

#include <cons.h>


#include <bitset>




// Rendering
// Physics
// GUI


int main() {

	std::cout << "Start : " << std::endl;

	uint16_t x = 16;
	uint16_t y = 13;
	uint32_t xy = x | (y << 16);



	std::cout << "XY: " << xy << " BITS : " << std::bitset<32>(xy) << std::endl;

//	OgreBites::ApplicationContext ctx(INSTANCE_NAME);
//	ctx.initApp();

	std::cout << "Ogre started : " << std::endl;

	/*slughorn::Atlas atlas;

	slughorn::freetype::loadAsciiFont("C:/Users/lenovo/source/repos/mHorseShoeeVCmake/mHorseShoe/src/slughorn/viaoda_libre.ttf", atlas);

	atlas.build();

	if (atlas.isBuilt())
	{
		std::cout << "ATLAS BUILT!!!!!" << std::endl;

		auto texData = atlas.getCurveTextureData();
		auto bandData = atlas.getBandTextureData();

		std::ofstream texOut("TexData.txt");
		std::ofstream bandOut("BandData.txt");
		for (auto& byte: texData.bytes)
		{
			texOut << byte;
		}
		for (auto& byte: bandData.bytes)
		{
			bandOut << byte;
		}

		auto shape = atlas.getCompositeShape(slughorn::Key::Key("A"));
		
		
		texOut.close();
		bandOut.close();

		
	}*/

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

	CaseHandler* caseHandler = new CaseHandler(monster);
	caseHandler->loadSlugObject("C:/Users/Administrator/src/repo/mHorseShoe/src/slughorn/viaoda_libre.ttf");
	// Physics INIT
	/*kint = new Kint();
	kint->InitPhysics();*/
	//std::filesystem::path skyBox = std::filesystem::path("../../../src/monster/shaders/skybox.glb");
	//fastgltf::Asset* asset = ResourceHandler::GetInstance()->loadGltfFile(skyBox);
	//std::vector<hRes::Mesh> skyBoxMesh = ResourceHandler::GetInstance()->generateMesh(*asset);
	

//	feel->initFeel(monster->sdlWindow,
	//GDHANDLER
	
	//GDHandler* gdHandler = new GDHandler(ResourceHandler::GetInstance(),monster,feel); // TODO integrate Feel with GDHandler itself

	//monster->setGrid();
	

	// TODO SETUP BETTER STARTUP
	//std::cout << "Setting up Skybox" << std::endl;
	//monster->setSkyBox();
	//monster->setGrid();

	//monster->_createGrassBlade(0.3, 1);

	Gui* gui = new Gui(ResourceHandler::GetInstance(), caseHandler);
	gui->_debugStats = &monster->imDebugStats;
	gui->mVulkan = monster;
	gui->initGuiComponents();
	

	// MAIN LOOP
	
	while (!Feel::GetInstance()->mappedEvents.quitApplication->eventState) {

		Glock::GetInstance()->setStartTime();

		Feel::GetInstance()->updateFeel();

		monster->updateMonster(Feel::GetInstance()->getCameraKeyInput(monster->keyboardCapture), Feel::GetInstance()->getCameraMouseInput(),Glock::GetInstance()->deltaTime);

		monster->startRenderingTillGui(Glock::GetInstance()->getShaderTime());
		gui->updateGuiComponents();
		monster->endRendering();


		Glock::GetInstance()->setEndTime();

		//std::cout << glock->getShaderTime() << std::endl;
		
	}

	std::cout << "loop ended : " << std::endl;

//	ResourceHandler::GetInstance()->saveResources();

	// Shutdown
	//kint->Shutdown();
	monster->Shutdown();

	//ResourceHandler::GetInstance()->shutdown();

	std::cout << "HEllOS";

	
}

     


#include "Gui.h"



void Gui::initGui(Ogre::ImGuiOverlay* overlay) {

	overlay->setZOrder(300);
	overlay->show();

	imOverlay = overlay;

	std::cout << "ImGui version" << ImGui::GetCurrentContext() << std::endl;

	this->viewport = ImGui::GetMainViewport();

	//resourceHandler->updateOgreMaterials();

	this->initGuiComponents();

}


void Gui::updateGui()
{

	//IKEYS* inputKeys = InputHandler::GetInstance()->getInputKeys();

	imOverlay->NewFrame();

	

	updateGuiComponents();

	

	ImGui::EndFrame();
}

void Gui::shutdown()
{
}


// NEW FRAMEWORK
void Gui::initGuiComponents()
{
	// Models
	SceneTabModelComponent* scnTabModel = new SceneTabModelComponent("Scene tab Model");
	this->addModelComponent(scnTabModel);

	//ResourceTabModelComponent* resourceTabModel = new ResourceTabModelComponent("Resource Tab Model");
	//this->addModelComponent(resourceTabModel);

	///*StatusTabModelComponent* statusModel = new StatusTabModelComponent("Status tab Model");
	//this->addModelComponent(statusModel);*/

	//AddTabModelComponent* addModel = new AddTabModelComponent("Add tab Model");
	//this->addModelComponent(addModel);

	ObjectTabModelComponent* objectModel = new ObjectTabModelComponent("Object tab Model");
	this->addModelComponent(objectModel);

	//ScenePanelTabModelComponent* scenePanelModel = new ScenePanelTabModelComponent("Scene Panel tab Model");
	//this->addModelComponent(scenePanelModel);

	RSUSTabModelComponent* rsusModel = new RSUSTabModelComponent("Rsus tab Model");
	this->addModelComponent(rsusModel);

	TestingTabModelComponent* testModel = new TestingTabModelComponent("Testing tab model");
	this->addModelComponent(testModel);

	//ToastTabModelComponent* toastModel = new ToastTabModelComponent("Toast tab model");
	//this->addModelComponent(toastModel);

	// Views
	SceneTabComponent* scnTab = new SceneTabComponent("Scene Tab",scnTabModel);
	this->addViewComponent(scnTab);

	//ResourceTabComponent* resourceTab = new ResourceTabComponent("Resource Tab", resourceTabModel);
	//this->addViewComponent(resourceTab);

	///*StatusTabComponent* statusTab = new StatusTabComponent("Status Tab", statusModel);
	//this->addViewComponent(statusTab);*/

	//AddTabComponent* addTab = new AddTabComponent("Add Tab", addModel);
	//this->addViewComponent(addTab);

	ObjectTabComponent* objectTab = new ObjectTabComponent("Object Tab", objectModel);
	this->addViewComponent(objectTab);

	//ScenePanelTabComponent* scenePanelTab = new ScenePanelTabComponent("Scene Panel Tab", scenePanelModel);
	//this->addViewComponent(scenePanelTab);

	RSUSTabComponent* rsusTab = new RSUSTabComponent("RSUS tab", rsusModel);
	this->addViewComponent(rsusTab);

	//HUDComponent* hudTab = new HUDComponent("Hud tab");
	//this->addViewComponent(hudTab);

	TestingTabComponent* testingTab = new TestingTabComponent("Test tab", testModel);
	this->addViewComponent(testingTab);

	//ToastTabViewComponent* toastView = new ToastTabViewComponent("Toast tab", toastModel);
	//this->addViewComponent(toastView);

}

void Gui::updateGuiComponents()
{
	for (int i = 0; i < Views.size(); i++)
	{
		Views.at(i)->view();
	}
}

void Gui::setWindowGrabPoints(int WIDTH, int HEIGHT)
{
	guiWPos->TOP_RIGHT = ImVec2(WIDTH, 0);
	guiWPos->TOP_LEFT = ImVec2(0, 0);
	guiWPos->TOP_CENTER = ImVec2(WIDTH / 2, 0);

	guiWPos->RIGHT = ImVec2(WIDTH, HEIGHT / 2);
	guiWPos->LEFT = ImVec2(0, HEIGHT / 2);
	guiWPos->CENTER = ImVec2(WIDTH / 2, HEIGHT / 2);

	guiWPos->BOTTOM_RIGHT = ImVec2(WIDTH, HEIGHT);
	guiWPos->BOTTOM_LEFT = ImVec2(0, HEIGHT);
	guiWPos->BOTTOM_CENTER = ImVec2(WIDTH / 2, HEIGHT);
	
}
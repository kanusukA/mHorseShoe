#include "Gui.h"



void Gui::initGui(Ogre::ImGuiOverlay* overlay, GuiComponent* component) {
	
	std::cout << "DBUGGING - GUI START" << std::endl;
	this->guiComponent = component;

	overlay->setZOrder(300);
	overlay->show();

	imOverlay = overlay;
	std::cout << "DBUGGING - GUI MainViewport start" << std::endl;

	std::cout << "ImGui version" << ImGui::GetCurrentContext() << std::endl;
	

	this->viewport = ImGui::GetMainViewport();
	std::cout << "DBUGGING - GUI MainViewport end" << std::endl;

	resourceHandler->updateOgreMaterials();

	guiComponent->sceneTab->StaticScenes = SceneHandler::GetInstance()->getStaticScenes();
	guiComponent->sceneTab->DynamicScenes = SceneHandler::GetInstance()->getDynamicScenes();
	guiComponent->sceneTab->MeshScenes = SceneHandler::GetInstance()->getMeshScenes();

}


void Gui::setPlayerObserver(PlayerObserver* pObserver)
{
	this->playerObserver = pObserver;
}


void Gui::updateGui()
{
	this->GuiInput();


	IKEYS* inputKeys = InputHandler::GetInstance()->getInputKeys();

	imOverlay->NewFrame();

	if(inputKeys->ALL_TAB_KEY_T){
		if (inputKeys->STATUS_TAB_KEY_T) {
			_statusTab();
		}
		if (inputKeys->ADD_OBJECT_KEY_T) {
			_addTab();
		}
		if (inputKeys->RESOURCE_TAB_KEY_T) {
			_resourceTab();
		}
		if (inputKeys->LIGHT_TAB_KEY_T) {
			_lightTab();
		}

		_RSUSTab();

		_diegnos();

		_objectTab();

		_HUD();

		_debugTab();

		_objectsPanel();

		_heightMapTab();

		_terrainTab();

		_SceneTab();

	}

	ImGui::EndFrame();
}

void Gui::shutdown()
{
}

void Gui::_resourceTab()
{
	ImGui::Begin("RESOURCES");

	ImGui::Columns(2);

	if ( ImGui::Button("Get All Resources") ) {
		this->refreshResources();
	}

	ImGui::Spacing();

	// Add resources
	ImGui::Text("RENDER MESHES : ");
	ImGui::Spacing();
	if (resourceHandler->renderMeshes->empty()) {
		ImGui::Text("No Render Meshes Found");
	}
	else {
		for (int i = 0; i < resourceHandler->renderMeshes->size(); i++)
		{
			ImGui::Text(resourceHandler->renderMeshes->at(i).filename().string().c_str());
		}
		
	}

	ImGui::Spacing();

	ImGui::Text("COLLIDER MESHES : ");
	ImGui::Spacing();
	if (resourceHandler->colliderMeshes->empty()) {
		ImGui::Text("No Collider Meshes Found");
	}
	else {
		for (int i = 0; i < resourceHandler->colliderMeshes->size(); i++)
		{
			ImGui::Text(resourceHandler->colliderMeshes->at(i).filename().string().c_str());
		}
	}

	ImGui::Spacing();

	ImGui::Text("MATERIALS : ");
	ImGui::Spacing();
	if (resourceHandler->meshMaterials->empty()) {
		ImGui::Text("No MATERIALS Found");
	}
	else {
		for (int i = 0; i < resourceHandler->meshMaterials->size(); i++)
		{
			ImGui::Text(resourceHandler->meshMaterials->at(i).filename().string().c_str());
		}
	}

	ImGui::Text("IMAGES : ");
	ImGui::Spacing();
	if (resourceHandler->images->empty()) {
		ImGui::Text("No IMAGES Found");
	}
	else {
		for (int i = 0; i < resourceHandler->images->size(); i++)
		{
			ImGui::Text(resourceHandler->images->at(i).filename().string().c_str());
		}
	}

	ImGui::NextColumn();

	if (ImGui::Button("Render Meshes to Ogre"))
	{
		resourceHandler->addOgreRenderMeshResourceLocation();
	}

	ImGui::Spacing();

	ImGui::Text("Monster Meshes :");

	ImGui::Spacing();

	if(resourceHandler->ogreRenderMeshes){
		for (int j = 0; j < resourceHandler->ogreRenderMeshes.get()->size(); j++)
		{
			ImGui::Text(resourceHandler->ogreRenderMeshes.get()->at(j).c_str());
		}
	}
	else {
		
		ImGui::Text("No ZAZA");
	}

	ImGui::End();

}

void Gui::_statusTab()
{
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(250, 100));
	
	ImGui::Begin("STATUS",0,ImGuiWindowFlags_NoMove && ImGuiWindowFlags_NoCollapse && ImGuiWindowFlags_NoResize);

	ImGui::Text("Position");
	ImGui::Value("X", this->playerObserver->getPlayerPosition().x);
	ImGui::SameLine();
	ImGui::Value("Y", this->playerObserver->getPlayerPosition().y);
	ImGui::SameLine();
	ImGui::Value("Z", this->playerObserver->getPlayerPosition().z);

	ImGui::Text("Rotation");
	ImGui::Value("X", this->playerObserver->getPlayerRotation().getPitch().valueAngleUnits());
	ImGui::SameLine();
	ImGui::Value("Y", this->playerObserver->getPlayerRotation().getYaw().valueAngleUnits());
	ImGui::SameLine();
	ImGui::Value("Z", this->playerObserver->getPlayerRotation().getRoll().valueAngleUnits());

	ImGui::Value("FPS", *this->fps);


	ImGui::End();

}

void Gui::_heightMapTab()
{
	ImGui::Begin("HEIGHT MAP");

	if (resourceHandler->images->empty()) {
		ImGui::Text("No Images Loaded");
	}
	else {
		if (!resourceHandler->images->empty()) {
			if (ImGui::BeginCombo("Images", resourceHandler->images->at(guiComponent->heightMapTab->imgPos).filename().string().c_str())) {

				for (int i = 0; i < resourceHandler->images->size(); i++)
				{
					if (ImGui::Selectable(resourceHandler->images->at(i).filename().string().c_str(), i == guiComponent->heightMapTab->imgPos))
					{
						guiComponent->heightMapTab->imgPos = i;

						Ogre::TextureManager::getSingleton().load(resourceHandler->images->at(i).filename().string().c_str(), "Images");
						Ogre::Texture* imgTex = Ogre::TextureManager::getSingleton().getByName(resourceHandler->images->at(i).filename().string().c_str(), "Images").get();
						hdl = imgTex->getHandle();

						guiComponent->heightMapTab->width = imgTex->getWidth();
						guiComponent->heightMapTab->height = imgTex->getHeight();

					}

				}
				ImGui::EndCombo();
			}
			

			if (hdl) {
				ImGui::Image((ImTextureID)hdl, ImVec2(250, 250));

				ImGui::Spacing();

				ImGui::SliderFloat("Displacement", guiComponent->heightMapTab->displacementFac, 0.0f, 100.0f);

				ImGui::InputInt("Vertex Size", guiComponent->heightMapTab->vertSize);

				ImGui::InputInt("Grass Density", guiComponent->heightMapTab->grassDensity);

			}

			if (ImGui::BeginCombo("Grass length", resourceHandler->images->at(guiComponent->heightMapTab->grassImgPos).filename().string().c_str())) {

				for (int i = 0; i < resourceHandler->images->size(); i++)
				{
					if (ImGui::Selectable(resourceHandler->images->at(i).filename().string().c_str(), i == guiComponent->heightMapTab->grassImgPos))
					{
						guiComponent->heightMapTab->grassImgPos = i;

						Ogre::TextureManager::getSingleton().load(resourceHandler->images->at(i).filename().string().c_str(), "Images");
						Ogre::Texture* grassImgTex = Ogre::TextureManager::getSingleton().getByName(resourceHandler->images->at(i).filename().string().c_str(), "Images").get();
						grassHdl = grassImgTex->getHandle();

					}

				}
				ImGui::EndCombo();
			}
			
			if (grassHdl) {

				ImGui::Image((ImTextureID)grassHdl, ImVec2(250, 250));

				ImGui::InputFloat("Scale", guiComponent->heightMapTab->grassScale);

				// add grass height

				if (ImGui::Button("Set Terrain and Grass"))
				{
					guiComponent->setHeightMap();
				}

			}
			


		}
	}

	ImGui::End();

}

void Gui::_addTab()
{
	ImGui::Begin("ADD");
	ImGui::InputText("Add Name",this->guiComponent->getObjectName());

	//SceneNode
	if (guiComponent->sceneTab->selectedScenes) {
		if (guiComponent->sceneTab->selectedScenes->size() > 0)
		{
			if (ImGui::BeginCombo("Scene", guiComponent->sceneTab->selectedScenes->at(guiComponent->sceneTab->selectedScenePos)->getName().c_str())) {
				for (int i = 0; i < guiComponent->sceneTab->selectedScenes->size(); i++)
				{
					if (ImGui::Selectable(guiComponent->sceneTab->selectedScenes->at(i)->getName().c_str(), i == guiComponent->sceneTab->selectedScenePos)) {
						guiComponent->sceneTab->selectedScenePos = i;
					}
				}
				ImGui::EndCombo();
			}
		}
		else {
			ImGui::Text("No Scene Node Found");
		}
	}
	else {
		ImGui::Text("No Scene Node Found");
	}

	ImGui::InputText("Scene Name", &guiComponent->sceneTab->CreateSceneNode);
	if (ImGui::Button("Create Scene"))
	{
		if (this->guiComponent->getPhysicsType() == StuffType::STUFF_DYNAMIC)
		{
			SceneHandler::GetInstance()->CreateScene(SceneType::DYNAMIC, guiComponent->sceneTab->CreateSceneNode);
			guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->DynamicScenes;
		}
		if (this->guiComponent->getPhysicsType() == StuffType::STUFF_STATIC)
		{
			SceneHandler::GetInstance()->CreateScene(SceneType::STATIC, guiComponent->sceneTab->CreateSceneNode);
			guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->StaticScenes;
		}
		if (this->guiComponent->getPhysicsType() == StuffType::STUFF_MESH_ONLY)
		{
			SceneHandler::GetInstance()->CreateScene(SceneType::MESH, guiComponent->sceneTab->CreateSceneNode);
			guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->MeshScenes;
		}
		guiComponent->sceneTab->selectedScenePos = 0;
		
	}



	//Null Check
	if(resourceHandler->ogreRenderMeshes) {
		// size check
		if(resourceHandler->ogreRenderMeshes.get()->size() > 0){
			// Add Monster render objects
			if (ImGui::BeginCombo("Render Mesh", resourceHandler->ogreRenderMeshes.get()->at(renderMeshesPosition).c_str())) {
				for (int i = 0; i < resourceHandler->ogreRenderMeshes.get()->size(); i++)
				{
					if (ImGui::Selectable(resourceHandler->ogreRenderMeshes.get()->at(i).c_str(), i == renderMeshesPosition)) {
						renderMeshesPosition = i;
					}
				}
				ImGui::EndCombo();
			}
		}

	}
	else {
		ImGui::Text("No Render Mesh Found.");
	}

	// Collider mesh
	if (ImGui::BeginCombo("Collider Mesh", guiComponent->getColliderMeshName()->c_str())) {
		if (ImGui::Selectable("Box", colliderMeshesPosition = -1)) {
			guiComponent->setColliderMeshName("box");
		}
		if (resourceHandler->colliderMeshes) {
			if (resourceHandler->colliderMeshes->size() > 0) {
				for (int i = 0; i < resourceHandler->colliderMeshes->size(); i++)
				{
					if (ImGui::Selectable(resourceHandler->colliderMeshes->at(i).filename().string().c_str(), i == colliderMeshesPosition)) {
						guiComponent->setColliderMeshName(resourceHandler->colliderMeshes->at(i).filename().string().c_str());
					}
				}
			}
		}
		ImGui::EndCombo();
	}
	
	
	ImGui::Text("PhysX Type");

	if (ImGui::RadioButton("DYNAMIC", this->guiComponent->getPhysicsType() == StuffType::STUFF_DYNAMIC))
	{
		this->guiComponent->setPhysicsType(StuffType::STUFF_DYNAMIC);
		guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->DynamicScenes;
		guiComponent->sceneTab->selectedScenePos = 0;
	}
	if (ImGui::RadioButton("STATIC", this->guiComponent->getPhysicsType() == StuffType::STUFF_STATIC))
	{
		this->guiComponent->setPhysicsType(StuffType::STUFF_STATIC);
		guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->StaticScenes;
		guiComponent->sceneTab->selectedScenePos = 0;
	}
	if (ImGui::RadioButton("MESH", this->guiComponent->getPhysicsType() == StuffType::STUFF_MESH_ONLY))
	{
		this->guiComponent->setPhysicsType(StuffType::STUFF_MESH_ONLY);
		guiComponent->sceneTab->selectedScenes = guiComponent->sceneTab->MeshScenes;
		guiComponent->sceneTab->selectedScenePos = 0;
	}

	if (this->guiComponent->getPhysicsType() == StuffType::STUFF_DYNAMIC) {
		ImGui::InputInt("Mass", this->guiComponent->getMass());
		//ImGui::InputInt3("Collider Size", this->guiComponent->getColliderSize());

	}else if (this->guiComponent->getPhysicsType() == StuffType::STUFF_STATIC) {
		ImGui::InputInt3("Collider Size", this->guiComponent->getColliderSize());
	}
	

	ImGui::InputInt3("Position", this->guiComponent->getPosition());

	ImGui::InputInt3("Rotation", this->guiComponent->getRotation());

	if (ImGui::Button("ADD")) {
		if (guiComponent->sceneTab->selectedScenes) {
			if (guiComponent->sceneTab->selectedScenes->size() > 0) {
				this->guiComponent->setRenderMeshName(resourceHandler->ogreRenderMeshes.get()->at(renderMeshesPosition));
				this->guiComponent->addObject();
			}
		}
		else {
			std::cout << "Error creating Object" << std::endl;
		}
	}

	ImGui::End();
}

void Gui::_debugTab()
{
	ImGui::Begin("Debug");
	if (ImGui::Button("ShowRender"))
	{
		this->guiComponent->showRenderMeshes();
	}
	if (ImGui::Button("ShowDebugLines"))
	{
		this->guiComponent->showDebugLines();
	}
	ImGui::End();
}

void Gui::_objectsPanel()
{
	// Objects
	ImGui::Begin("Stuffs");

	ImGui::Text("Dynamic");
	ImGui::Spacing();

	if (guiComponent->sceneTab->DynamicScenes) {
		if (guiComponent->sceneTab->DynamicScenes->size() > 0) {
			for (int i = 0; i < guiComponent->sceneTab->DynamicScenes->size(); i++)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(guiComponent->sceneTab->DynamicScenes->at(i)->getName().c_str());
				if (guiComponent->sceneTab->DynamicScenes->at(i)->getAttachedObjects().size() > 0) {
					for (int j = 0; j < guiComponent->sceneTab->DynamicScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(guiComponent->sceneTab->DynamicScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}

				}
			}
		}
	}

	ImGui::Spacing();
	ImGui::Text("Static");
	ImGui::Spacing();

	if (guiComponent->sceneTab->StaticScenes) {
		if (guiComponent->sceneTab->StaticScenes->size() > 0) {
			for (int i = 0; i < guiComponent->sceneTab->StaticScenes->size(); i++)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(guiComponent->sceneTab->StaticScenes->at(i)->getName().c_str());
				if (guiComponent->sceneTab->StaticScenes->at(i)->getAttachedObjects().size() > 0) {
					for (int j = 0; j < guiComponent->sceneTab->StaticScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(guiComponent->sceneTab->StaticScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}

				}
			}
		}
	}

	ImGui::Spacing();
	ImGui::Text("Mesh");
	ImGui::Spacing();

	if (guiComponent->sceneTab->MeshScenes) {
		if(guiComponent->sceneTab->MeshScenes->size() > 0){
			for (int i = 0; i < guiComponent->sceneTab->MeshScenes->size(); ++i)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(guiComponent->sceneTab->MeshScenes->at(i)->getName().c_str());
				if (guiComponent->sceneTab->MeshScenes->at(i)->getAttachedObjects().size() > 0) {
					for (int j = 0; j < guiComponent->sceneTab->MeshScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(guiComponent->sceneTab->MeshScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}

				}
			}
		}
	}

	ImGui::End();

}

void Gui::_lightTab()
{
	ImGui::Begin("Lights");

	ImGui::Text("Main Directional Light");
	ImGui::Spacing();

	ImGui::InputInt3("Direction", guiComponent->lightTab->mDirection);

	ImGui::InputInt("Power Scale",&guiComponent->lightTab->powerScale);

	if (ImGui::Button("Add mdrl")) {
		guiComponent->setUpMainDirectionalLight();
	}

	ImGui::Spacing();
	ImGui::Text("Movable Light");

	ImGui::InputText("Light Name", guiComponent->lightTab->lightName);

	if (ImGui::BeginCombo("Light Type",guiComponent->lightTab->lightTypeNames[guiComponent->lightTab->lightTypePos]))
	{
		for (int i = 0; i < 4; i++)
		{
			ImGui::Selectable(guiComponent->lightTab->lightTypeNames[i], &guiComponent->lightTab->lightTypePos);
		}
		ImGui::EndCombo();
	}

	ImGui::InputInt3("Light Position", guiComponent->lightTab->lightPos);
	ImGui::InputInt3("Light Direction", guiComponent->lightTab->lightDir);
	ImGui::InputInt("Light Power Scale", &guiComponent->lightTab->lightPowerScale);

	if (ImGui::Button("Add Light")) {
		guiComponent->setUpLight();
	}

	ImGui::End();
}

void Gui::_objectTab()
{

	if (guiComponent->getSelectedObject()->selectingName) {

		ImGui::SetNextWindowPos(ImVec2(0, 200));
		ImGui::SetNextWindowSize(ImVec2(250, 50));

		ImGui::Begin("Selectable", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::Text("Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getSelectedObject()->selectingName->c_str());

		ImGui::End();

		if(guiComponent->getSelectedObject()->selectedStuff){
			if (guiComponent->getSelectedObject()->selectedStuff->scnNode) {
				ImGui::SetNextWindowPos(ImVec2(0, 250));
				ImGui::SetNextWindowSize(ImVec2(250, 350));

				ImGui::Begin("Selected Object", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

				ImGui::Text("Name : ");
				ImGui::SameLine();
				ImGui::Text(guiComponent->getSelectedObject()->selectedStuff->name.c_str());

				ImGui::Text("Position : ");
				ImGui::Text("X : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(guiComponent->getSelectedObject()->selectedStuff->scnNode->getPosition().x).c_str());
				ImGui::Text("Y : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(guiComponent->getSelectedObject()->selectedStuff->scnNode->getPosition().y).c_str());
				ImGui::Text("Z : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(guiComponent->getSelectedObject()->selectedStuff->scnNode->getPosition().z).c_str());

				ImGui::Text("Rotation : ");
				ImGui::Text("X : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(getObjRotation(guiComponent->getSelectedObject()->selectedStuff->scnNode).x).c_str());
				ImGui::Text("Y : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(getObjRotation(guiComponent->getSelectedObject()->selectedStuff->scnNode).y).c_str());
				ImGui::Text("Z : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(getObjRotation(guiComponent->getSelectedObject()->selectedStuff->scnNode).z).c_str());

				if (ImGui::Checkbox("Cast Shadow", guiComponent->getSelectedObject()->shadowCasting)) {
					guiComponent->getSelectedObject()->selectedStuff->setCastShadow(*guiComponent->getSelectedObject()->shadowCasting);
				}

				if (ImGui::Button("Delete")) {
					guiComponent->deleteSelectedObj();
				}
				
				ImGui::End();
				
			}
		}

		
		
	}

	
}

void Gui::_HUD()
{

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	
	draw->AddLine(ImVec2(viewport->GetCenter().x, viewport->GetCenter().y - 50), 
		ImVec2(viewport->GetCenter().x, viewport->GetCenter().y + 50), ImColor(120, 50, 120));

	draw->AddLine(ImVec2(viewport->GetCenter().x - 50, viewport->GetCenter().y ),
		ImVec2(viewport->GetCenter().x + 50, viewport->GetCenter().y), ImColor(120, 50, 120));


}

void Gui::_diegnos()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::SetNextWindowPos(ImVec2(1400, 0));

	ImGui::Begin("Diagnostics", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ImGui::Text("FPS : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(*guiComponent->getDiegnostics()->fps).c_str());

	ImGui::Text("Render Time : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(*guiComponent->getDiegnostics()->renderFrameTime).c_str());

	ImGui::Text("Total Frame Time : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(*guiComponent->getDiegnostics()->TotalFrameTime).c_str());

	ImGui::End();
}

void Gui::_RSUSTab()
{
	ImGui::Begin("Shaders");

	// select material to send to RSUS
	if (resourceHandler->OgreMaterials)
	{
		if (ImGui::BeginCombo("Materials", resourceHandler->OgreMaterials->at(guiComponent->getRSUSParam()->selectedMaterial).c_str())) {
			for (int i = 0; i < resourceHandler->OgreMaterials->size(); i++)
			{

				if (ImGui::Selectable(resourceHandler->OgreMaterials->at(i).c_str(), guiComponent->getRSUSParam()->selectedMaterial == i))
				{
					guiComponent->getRSUSParam()->selectedMaterial = i;
				}

			}
			ImGui::EndCombo();
		}
		

		if (ImGui::Button("Shade it")) {
			guiComponent->sendMaterialRsus();
		}
	}
	else {
		ImGui::Text("No Materials Found");
	}

	if (ImGui::Button("Get Materials")) {
		resourceHandler->updateOgreMaterials();
	}


	
	if (guiComponent->getRSUSParam()->rsusObj) {

		ImGui::Text(" Fragment Shader Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->fragShaderName.c_str());

		ImGui::Text(" Fragment Shader File Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->fragShaderName.c_str());

		RSUShader* shade = guiComponent->getRSUSParam()->rsusObj;

		// Variables
		for (int i = 0; i < guiComponent->getRSUSParam()->rsusObj->fragVariables.size(); i++)
		{

			ShaderVarType type = shade->fragVariables.at(i).varType;

			switch (type)
			{
			case INTEGER:
				ImGui::InputInt(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varInt);
				break;
			case FLOAT0:
				if (ImGui::SliderFloat(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat);
				}
				break;
			case FLOAT2:
				if (ImGui::SliderFloat2(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat2, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat2(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat2);
				}
				break;
			case FLOAT3:
				if (ImGui::SliderFloat3(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat3, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat3(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat3);
				}
				break;
			case FLOAT4:
				if (ImGui::SliderFloat4(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat4, -1.0f, 1.0f)) {
					Ogre::Vector4 slid = Ogre::Vector4(shade->fragVariables.at(i).varFloat4[0], shade->fragVariables.at(i).varFloat4[1], shade->fragVariables.at(i).varFloat4[2], shade->fragVariables.at(i).varFloat4[3]);
					guiComponent->updateFragRsusFloat4(shade->fragVariables.at(i).varName, slid);
				}
				break;
			default:
				ImGui::Text("Invalid Type : ");
				ImGui::SameLine();
				ImGui::Text(shade->fragVariables.at(i).varName.c_str());
				break;
			}
			
		}

		ImGui::Spacing();

		ImGui::Text(" Vertex Shader Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->vertShaderName.c_str());

		ImGui::Text(" Vertex Shader File Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->vertShaderName.c_str());

		// Variables
		for (int i = 0; i < guiComponent->getRSUSParam()->rsusObj->vertVariables.size(); i++)
		{

			ShaderVarType type = shade->vertVariables.at(i).varType;

			switch (type)
			{
			case INTEGER:
				ImGui::InputInt(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varInt);
				break;
			case FLOAT0:
				if (ImGui::SliderFloat(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat, -1.0f, 10.0f)) {
					guiComponent->updateVertRsusFloat(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat);
				}
				break;
			case FLOAT2:
				if (ImGui::SliderFloat2(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat2, -1.0f, 10.0f)) {
					guiComponent->updateVertRsusFloat2(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat2);
				}
				break;
			case FLOAT3:
				if (ImGui::SliderFloat3(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat3, -1.0f, 1.0f)) {
					guiComponent->updateVertRsusFloat3(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat3);
				}
				break;
			case FLOAT4:
				if (ImGui::SliderFloat4(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat4, -1.0f, 1.0f)) {
					Ogre::Vector4 slid = Ogre::Vector4(shade->vertVariables.at(i).varFloat4[0], shade->vertVariables.at(i).varFloat4[1], shade->vertVariables.at(i).varFloat4[2], shade->vertVariables.at(i).varFloat4[3]);
					guiComponent->updateVertRsusFloat4(shade->vertVariables.at(i).varName, slid);
				}
				break;
			default:
				ImGui::Text("Invalid Type : ");
				ImGui::SameLine();
				ImGui::Text(shade->vertVariables.at(i).varName.c_str());
				break;
			}


		}

		ImGui::Spacing();
		if (ImGui::Button("Save")) {
			guiComponent->saveRsusObj();
		}

	}

	ImGui::End();

}


void Gui::GuiInput()
{
	//IKEYS* inputKeys = InputHandler::GetInstance()->getInputKeys();

	//// single key press mechanism - FOR NOW ITS ONLY LIMITED TO FUNCTIONS THEMSELVES AND NOT INCLUDED IN KEYHANDLER

	//if (inputKeys->ADD_OBJECT_KEY ) {
	//	guiComponent->showGuiTab(!guiComponent->getGuiTabVisibility()->addObjectTab, GuiTabs::GUI_ADD_OBJECT_TAB);
	//}
	//if (inputKeys->STATUS_TAB_KEY) {
	//	if (this->playerObserver != NULL) {
	//		guiComponent->showGuiTab(true, GuiTabs::GUI_STATUS_TAB);
	//	}
	//	
	//}
	//if (inputKeys->RESOURCE_TAB_KEY) {
	//	guiComponent->showGuiTab(true, GuiTabs::GUI_RESOURCES_TAB);
	//}
	//if (inputKeys->LIGHT_TAB_KEY) {
	//	guiComponent->showGuiTab(true, GuiTabs::GUI_LIGHT_TAB);
	//}
	//if (inputKeys->ALL_TAB_KEY) {
	//	guiComponent->showGuiTab(false, GuiTabs::GUI_ALL_TABS);
	//}
	//else
	//{
	//	guiComponent->showGuiTab(true, GuiTabs::GUI_ALL_TABS);
	//}

	
	
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
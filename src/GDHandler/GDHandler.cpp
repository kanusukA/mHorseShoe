#include "GDHandler.h"

// Add newly made player Subject and observers
// Get this shit working

GDHandler::GDHandler(Monster* renderer, Kint* physics) {
	
	monster = renderer;

	// initalize monster
	monster->InitMonster();
	std::cout << "Monster window - SDL3 created!" << std::endl;
	// FEELS
	initFeel(monster->sdlWindow);

	this->stuffHandler = new StuffHandler(monster,physics);

	SceneHandler::GetInstance()->setStuffHandler(stuffHandler);


}

void GDHandler::initGui(Ogre::ImGuiOverlay* overlay, GuiComponent* guiComp) {

	gui->initGui(overlay, guiComp);
	std::cout << "setup Gui" << std::endl;
	// inits GDMEDIATOR
	// First it requires stuffhandler to mediate object functions
	// Stuff in used to create all types of objects used in horesshoes
	// As Such it requires both monster and kint reference.
	GDMediator* gdMediator = new GDMediator(stuffHandler);
	guiComp->setMediator(gdMediator);
	std::cout << "setup mediator" << std::endl;
	// setup objects
	guiComp->setStuffObjects(stuffHandler->getStuffDynamicVec(), stuffHandler->getStuffMeshVec(), stuffHandler->getStuffStaticVec());
	guiComp->setSelectableObject(stuffHandler->getSelectedObj());
	std::cout << "setup objects" << std::endl;
	this->guiComponent = guiComp;
}

// Fetches Resources 
void GDHandler::preSetup()
{
	// Resource 
	
	SceneObject obj = SceneObject();
	obj.name = "Cube";
	
	//ResourceHandler::GetInstance()->saveSceneObject("Objects", obj);


	//monster->createTerrain(Ogre::Vector2(50, 50), 1, 2);
	//monster->createGrass();
	//monster->createGrass();
	//stuffHandler->addGrass(1000);

	/*stuffHandler->addObject(
		"grass",
		"GraggPlane.mesh",
		"box",
		Ogre::Vector3(5, 5, 5),
		Ogre::Vector3(90, 0, 0),
		StuffType::STUFF_MESH_ONLY,
		1,
		Ogre::Vector3(0, 0, 0),
		false,
		false
	);*/

	//// Add Ground
	/*stuffHandler->addObject(
		"Ground",
		"main_ground.mesh",
		"box",
		Ogre::Vector3(0, 0, 0),
		Ogre::Vector3(0, 0, 0),
		STUFF_STATIC,
		1,
		Ogre::Vector3(100, 1, 100)
	);*/

}

void GDHandler::addPlayerNode()
{
	if (this->playerSubject == nullptr) {
		this->playerSubject = new PlayerSubject(monster->getPlayerSceneNode(),monster->getCamera());
	}
	
	PlayerObserver* playerObserver = new PlayerObserver(this->playerSubject);
	// Show Gui Player info
	gui->setPlayerObserver(playerObserver);


	
}






//bool GDHandler::addObject()
//{
//	if (ap->type == phyType::DYNAMIC) {
//
//		PxTransform transform = PxTransform(PxVec3(ap->pos[0], ap->pos[1], ap->pos[2]));
//		PxGeometry* geo = new PxBoxGeometry(PxVec3(ap->c_size[0], ap->c_size[1], ap->c_size[2]));
//
//		createStuffRigidDynamic(*ap->add_name,*ap->msh_name,transform,*ap->mass,geo);
//
//		addVisualShape();
//		return true;
//		
//	}
//	else if (ap->type == phyType::STATIC)
//	{
//		PxTransform transform = PxTransform(PxVec3(ap->pos[0], ap->pos[1], ap->pos[2]));
//		PxGeometry* geo = new PxBoxGeometry(PxVec3(ap->c_size[0], ap->c_size[1], ap->c_size[2]));
//
//		createStuffRigidStatic(*ap->add_name, *ap->msh_name, transform, geo);
//
//		return true;
//
//	}
//	else {
//		// MESH INSTANCE 
//		// TODO
//	}
//
//	return false;
//}


// Transfer into Stuff
//void GDHandler::createStuffRigidDynamic(std::string name, Ogre::String meshName, PxTransform position, PxReal mass, PxGeometry* geo)
//{
//	StuffDynamic* sDyn = new StuffDynamic();
//	sDyn->setPxRigidDynamic(createRigidBody(name, position, mass, geo));
//	sDyn->setSceneNode(monster->loadMeshScnNode(name, meshName));
//
//	sDynColl.push_back(sDyn);
//}
//void GDHandler::createStuffRigidDynamic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxReal mass, PxGeometry* geo)
//{
//	StuffDynamic* sDyn = new StuffDynamic();
//	sDyn->setPxRigidDynamic(createRigidBody(name, position, mass, geo));
//	sDyn->setSceneNode(sNode);
//
//	sDynColl.push_back(sDyn);
//}
//
//
//void GDHandler::createStuffRigidStatic(std::string name, Ogre::String meshName, PxTransform position, PxGeometry* geo)
//{
//	StuffStatic* sStatic = new StuffStatic();
//	sStatic->setPxRigidStatic(createStaticBody(name, position, geo));
//	sStatic->setSceneNode(monster->loadMeshScnNode(name, meshName));
//
//	sStaticColl.push_back(sStatic);
//}
//void GDHandler::createStuffRigidStatic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxGeometry* geo)
//{
//	StuffStatic* sStatic = new StuffStatic();
//	sStatic->setPxRigidStatic(createStaticBody(name, position, geo));
//	sStatic->setSceneNode(sNode);
//
//	sStaticColl.push_back(sStatic);
//}


void GDHandler::updateGDHandler(float deltaTime)
{
	// INPUT
	updateInput(deltaTime);
	// PLAYER UPDATE
	_updatePlayerPrameters(deltaTime);


	// UPDATE GUI
	gui->updateGui();
	// UPDATE PHYSICS AND RENDERING
	this->stuffHandler->update(deltaTime);

}

void GDHandler::_updatePlayerPrameters(float deltaTime)
{
	// PLAYER MOVEMENT
	// Updates the player Scene node set in the monster player Node
	// Updates the playerSubject which in turn updates all the assigned Observers
	if (playerSubject != nullptr && mouseHidden) {
		updateRotation(deltaTime, playerSubject);
		updatePosition(deltaTime, playerSubject);
	}

}



void GDMediator::_addObject(GuiComponent* component) const 
{

	// Extract Component
	Ogre::Vector3 position = Ogre::Vector3(
		component->getPosition()[0],
		component->getPosition()[1],
		component->getPosition()[2]
	);

	Ogre::Vector3 collider = Ogre::Vector3(
		component->getColliderSize()[0],
		component->getColliderSize()[1],
		component->getColliderSize()[2]
	);

	Ogre::Vector3 rotation = Ogre::Vector3(
		component->getRotation()[0],
		component->getRotation()[1],
		component->getRotation()[2]
	);

	stuffHandler->addObject(
		component->sceneTab->selectedScenes->at(component->sceneTab->selectedScenePos)->getName(),
		*component->getObjectName(),
		*component->getRenderMeshName(),
		*component->getColliderMeshName(),
		position,
		rotation,
		component->getPhysicsType(),
		*component->getMass(),
		collider
	);

}

void GDMediator::_addMDRL(GuiComponent* component) const
{
	Ogre::Vector3 dir = Ogre::Vector3(
		component->lightTab->mDirection[0],
		component->lightTab->mDirection[1],
		component->lightTab->mDirection[2]
	);

	stuffHandler->addMDRL(
		*component->lightTab->mdrlName,
		dir,
		component->lightTab->powerScale
	);
}

void GDMediator::_addLight(GuiComponent* component) const
{
	Ogre::Vector3 dir = Ogre::Vector3(
		component->lightTab->lightDir[0],
		component->lightTab->lightDir[1],
		component->lightTab->lightDir[2]
	);
	Ogre::Vector3 pos = Ogre::Vector3(
		component->lightTab->lightPos[0],
		component->lightTab->lightPos[1],
		component->lightTab->lightPos[2]
	);
	

	stuffHandler->addLight(
		*component->lightTab->lightName,
		pos,
		dir,
		component->lightTab->lightPowerScale,
		Ogre::Light::LightTypes(component->lightTab->lightTypePos)
	);
}

void GDMediator::_addHeightMap(GuiComponent* component) const
{
	stuffHandler->setHeightMap(ResourceHandler::GetInstance()->images->at(component->heightMapTab->imgPos).filename().string().c_str(),
		ResourceHandler::GetInstance()->images->at(component->heightMapTab->grassImgPos).filename().string().c_str(),
		*component->heightMapTab->displacementFac,
		*component->heightMapTab->vertSize,
		*component->heightMapTab->grassDensity,
		*component->heightMapTab->grassScale
	);
}

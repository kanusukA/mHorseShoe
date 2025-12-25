#pragma once


#ifndef GDHANDLER_H
#define GDHANDLER_H

#include <GDHandler/StartPlatinum.h>

class GDHandler : public GDBuilderContext
{
public:

	CaseHandler* caseHandler;
	StuffHandler* stuffHandler;

	//PlayerSubject* playerSubject = nullptr;

	Gui* gui;

	GDHandler(ResourceHandler* resourceHan_p, Monster* renderer, Kint* physics, Feel* feel_p) : GDBuilderContext(resourceHan_p, renderer, physics, feel_p) {


		gui = new Gui(caseHandler, this->stuffHandler, ResourceHandler::GetInstance(), RSUS::GetInstance());

		gui->initGui(this->monster->imguiOverlay);

		//if (this->playerSubject == nullptr) {
		//	this->playerSubject = new PlayerSubject(renderer->getPlayerSceneNode(), renderer->getCamera());
		//}

		//PlayerObserver* playerObserver = new PlayerObserver(this->playerSubject);
		//// Show Gui Player info
		//gui->setPlayerObserver(playerObserver);

	};



	// Creates SceneNode Based on the name and Mesh.
	// If PhysicsType : eRIGID_DYNAMIC the same mesh is used for collision.
	// NOTE : SHAPE IS NOT CONSIDERED IN THE INFO.
	void createStuffRigidDynamic(std::string name, Ogre::String meshName, PxTransform position, PxReal mass, PxGeometry* geo) {};
	void createStuffRigidDynamic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxReal mass, PxGeometry* geo) {};

	void createStuffRigidStatic(std::string name, Ogre::String meshName, PxTransform position, PxGeometry* geo) {};
	void createStuffRigidStatic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxGeometry* geo) {};

	void update(float deltatime) override;


	// GDHANDLER (KINT) -> MONSTER STORAGE OF DATA
	//void addVisualShape();


};


// Mediator implementation
class GDMediator : public Mediator {
private:
	StuffHandler* stuffHandler = nullptr;

public:

	GDMediator(StuffHandler* stuff) {
		stuffHandler = stuff;
	}

	// Initalises GuiComponents and there values
	void initGuiComponents(MediatorComponent* component,InitEvents events);

	void NotifyView(InitEvents componentType, int compFuncEvent) const override {

	}

	void Notify(MediatorComponent* component, std::string event) const override {

	}


};

#endif // !GDHANDLER_H



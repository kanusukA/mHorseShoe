#pragma once


#ifndef GDHANDLER_H
#define GDHANDLER_H

#include <Gui/Gui.h>
#include <feel/Feel.h>


class GDHandler : public Feel
{
public:

	GDHandler(Monster* renderer, Kint* physics);

	StuffHandler* stuffHandler;

	Monster* monster;
	Gui* gui;

	PlayerSubject* playerSubject = nullptr;

	void initGui(Ogre::ImGuiOverlay* overlay);

	// Sets up ground and resource handler and stuff
	void preSetup();


	// Creates SceneNode Based on the name and Mesh.
	// If PhysicsType : eRIGID_DYNAMIC the same mesh is used for collision.
	// NOTE : SHAPE IS NOT CONSIDERED IN THE INFO.
	void createStuffRigidDynamic(std::string name, Ogre::String meshName, PxTransform position, PxReal mass, PxGeometry* geo) {};
	void createStuffRigidDynamic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxReal mass, PxGeometry* geo) {};

	void createStuffRigidStatic(std::string name, Ogre::String meshName, PxTransform position, PxGeometry* geo) {};
	void createStuffRigidStatic(std::string name, Ogre::SceneNode* sNode, PxTransform position, PxGeometry* geo) {};

	void updateGDHandler(float deltaTime);

	// GDHANDLER (KINT) -> MONSTER STORAGE OF DATA
	//void addVisualShape();

	void _updatePlayerPrameters(float deltaTime);

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



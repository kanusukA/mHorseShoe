#pragma once


#ifndef GDHANDLER_H
#define GDHANDLER_H

#include <GDHandler/StartPlatinum.h>
#include <feel/Feel.h>

class GDHandler : public CaseHandler
{

public:

	Feel* feel;

	PlayerSubject* playerSubject = nullptr;

	Gui* gui;

	GDHandler(ResourceHandler* resourceHan_p, Monster* renderer, Kint* physics, Feel* feel_p) : CaseHandler(renderer, physics) {

		// Init resources
		ResourceHandler::GetInstance();

		PlayerInput* pInput = new PlayerInput();
		playerSubject = new PlayerSubject(monster->getCameraScnNode(), monster->getCamera(), pInput);
		

		feel = feel_p;
		KeyHandler* keyHandler = new KeyHandler(this);
		keyHandler->setPlayerInput(pInput);

		feel->initFeel(renderer->sdlWindow, keyHandler);


		gui = new Gui(this, this->stuffHandler, ResourceHandler::GetInstance(), RSUS::GetInstance(),this);

		gui->initGui(this->monster->imguiOverlay);



		
	
		//this->setResources();

	};

	// RESOURCE WORK

	// Set Resources Adds all resources loaded by ResourceLoader into ResourceBuilderContext and generates their Resource Objects
	//void setResources();

	// OBJECT CREATION / GDCONTEXT OVERRIDE

	/*Ogre::SceneNode* createScene(std::string name_p) override;
	Ogre::Entity* createObject(std::string name_p, Ogre::MeshPtr mesh_p) override;
	Ogre::MaterialPtr createMaterial(std::string materialName_p) override;


	ResID addMaterial(std::string matname_p) override;
	ResID addShader(Ogre::MaterialPtr mat_p, ShaderType type) override;

	void setShaderVars(Shader* shader) override;

	void selectShader(Shader* shader_p) override;*/

	// Creates SceneNode Based on the name and Mesh.
	// If PhysicsType : eRIGID_DYNAMIC the same mesh is used for collision.
	// NOTE : SHAPE IS NOT CONSIDERED IN THE INFO.
	// OLD COMPONENTS DO NOT USE
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



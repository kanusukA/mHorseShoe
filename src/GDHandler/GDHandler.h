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
	GuiComponent* guiComponent;
	Gui* gui = new Gui();

	PlayerSubject* playerSubject = nullptr;

	void addPlayerNode();

	void initGui(Ogre::ImGuiOverlay* overlay, GuiComponent* guiComp);

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
	void _updateStuffs(float deltaTime);

};


// Mediator implementation
class GDMediator : public Mediator {
private:
	StuffHandler* stuffHandler = nullptr;

	void _addObject(GuiComponent* component) const; // Contains the conversion from GuiComponent to Stuff Object.
	void _addMDRL(GuiComponent* component) const;
	void _addLight(GuiComponent* component) const;
	void _addHeightMap(GuiComponent* component) const;

public:

	GDMediator(StuffHandler* stuff) {
		stuffHandler = stuff;
	}

	void Notify(MediatorComponent* component, std::string event) const override {
		GuiComponent* guiComp = static_cast<GuiComponent*>(component);
		if (event == GDEVENT_ADD_OBJECT) {
			try {
				
				this->_addObject(guiComp);
			}
			catch (const std::exception &e) {
				std::cout << "Error in GDMediator type conversion" << std::endl;
				std::cout << e.what() << std::endl;
			}
		}
		else if (event == GDEVENT_SHOW_RENDERING) {
			stuffHandler->showOgreRendering();
		}
		else if (event == GDEVENT_SHOW_PHYSX_VISUAL_DEBUG) {
			stuffHandler->showDebugPhysxMeshes();
		}
		else if (event == GDEVENT_ADD_MDRL) {
			
			this->_addMDRL(guiComp);
		}
		else if (event == GDEVENT_ADD_LIGHT) {
			
			this->_addLight(guiComp);
		}
		else if (event == GDEVENT_SET_HEIGHTMAP){
			
			this->_addHeightMap(guiComp);
		}
		else if (event == GDEVENT_DELETE_SELECTED_OBJ) {
			stuffHandler->deleteSelectedObj();
		}
		else if (event == GDEVENT_GENERATE_TERRAIN) {

			if (guiComp->terrainTab->selectedHeightMap > -1) {
				stuffHandler->createHeightmapTerrain(
					ResourceHandler::GetInstance()->images->at(guiComp->terrainTab->selectedHeightMap).filename().string().c_str(),
					ResourceHandler::GetInstance()->OgreMaterials->at(guiComp->terrainTab->selectedMaterial),
					guiComp->terrainTab->terrainSize,
					guiComp->terrainTab->blocks,
					guiComp->terrainTab->displacement,
					guiComp->terrainTab->scale

				);
			}
			else {
				stuffHandler->createTerrain(
					ResourceHandler::GetInstance()->OgreMaterials->at(guiComp->terrainTab->selectedMaterial),
					guiComp->terrainTab->terrainSize,
					guiComp->terrainTab->blocks
				);
			}
			

		}

	}


};

#endif // !GDHANDLER_H



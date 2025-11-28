#pragma once

#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <string>
#include <GDHandler/stuff/Stuff.h>

// GuiFRAMEWORK IMPLEMENTATION
// HERE ARE THE CLASS USED TO CREATE VIEW AND MODEL COMPONENTS FOR THE GUI

class ModelComponent;
class ViewComponent;

// GDSource - IT IS THE CONNECTOR CLASS FOR THE GUI FRAMEWORK.
// IT PROVIDES ACCESS TO OTHER PARTS OF THE SOFTWARE/
// THIS CLASS SHOULD NOT BE DIRCETLY CONNECTED WITH THE VIEW CLASS AS IT PROVIDES LIMITLESS ACCESS OF OTHER CLASSES
// TYPICALLY MODELCOMPONENT MUST BE USED IN BETWEEN TO FILTER AND MASK DATA.
class GDSource {

private:
	GDSource() {};

	SceneHandler* scnHandler = nullptr;
	StuffHandler* stuffHandler = nullptr;
	//Feel* feel = nullptr;

public:
	GDSource(SceneHandler* scnhan, StuffHandler* stuffhan) {
		this->scnHandler = scnhan;
		this->stuffHandler = stuffhan;
		//this->feel = feelhan;
	};
};


class GuiFramework :public GDSource {
	
protected:

	std::vector<ModelComponent*> Models = std::vector<ModelComponent*>();
	std::vector<ViewComponent*> Views = std::vector<ViewComponent*>();

	ModelComponent* getModelByName(std::string name) {

	};
	ModelComponent* getModelById(int id) {

	};

public:

	GuiFramework(SceneHandler* scnhan, StuffHandler* stuffhan) : GDSource(scnhan, stuffhan) {}

	// RUNS AT INIT, USED DURING THE CREATION OF VIEW COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// VIEW COMPONENTS MUST BE ADDED TO BE RENDERED!
	virtual void addViewComponent(ViewComponent* viewComponent) {};

	// RUNS AT INIT, USED DURING THE CREATION OF MODEL COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// MODEL COMPONENTS MUST BE ADDED TO BE RENDERED
	virtual void addModelComponent(ModelComponent* modelComponent) {};
		

};

// ViewComponent is used to create elements of UI, typically a window of ImGui
// name/id are used to access the derived class from the main Gui class
// use the view function to write renderable ImGui Functions
class ViewComponent {
protected:

	GuiFramework* guiFramework;

	bool show = true;
	int id = 0;
	const char* name = "";
	// Set type

public:

	// FRAMEWORK METHOD! NOT TO BE USED OUTSIDE
	void setFramework(GuiFramework* framework) {
		guiFramework = framework;
	}

	virtual void view() {};
};

// ModelComponent is the mediator layer between ViewComponent and GDSource
// It is a base class and stores data and functions relative to the view its attached with.
// A single ViewComponent may have multiple ModelComponents and ModelComponents can be shared among ViewComponents to provide
// data consistancy.
class ModelComponent {
protected:
	GuiFramework* guiFramework;
	GDSource* gdSource;

public:

	void setFramework(GuiFramework* framework) {
		guiFramework = framework;
	}
	void setSource(GDSource* source) {
		gdSource = source;
	}
	

};


// MAIN EVENT ENUM - CONTAINS ALL THE VIEW COMPONENETS
enum InitEvents
{
	SCENE_TAB_COMP

};

// COMPONENETS ENUMS - CONTAINS ENUM SPECIFIC FUNCTIONS
enum SceneTabComp
{
	REFRESH,
	SAVE_CASE,
	LOAD_CASE
};


// GUI -> GDHandler
// GDMediator Events
#define GDEVENT_ADD_OBJECT "gd_event_add_object"
#define GDEVENT_SHOW_PHYSX_VISUAL_DEBUG "show_physx_visual_debug"
#define GDEVENT_SHOW_RENDERING "show_ogre_render"
#define GDEVENT_DELETE_SELECTED_OBJ "delete_selected_obj"
#define GDEVENT_SHOW_PHYSX_RENDERING "show_physx_render"
#define GDEVENT_ADD_MDRL "add_main_directional_light"
#define GDEVENT_ADD_LIGHT "add_light_to_scene"
#define GDEVENT_SET_HEIGHTMAP "set_height_map"
#define GDEVENT_SEND_MAT_RSUS "send_material_to_rsus"
#define GDEVENT_GENERATE_TERRAIN "generate_terrain"
#define GDEVENT_GETSELECTEDOBJFRAGMENT "get_selected_obj_fragment_shader"

class MediatorComponent;

class Mediator {

public:
	virtual void Notify(MediatorComponent* component, std::string event) const = 0;
	virtual void NotifyView(InitEvents componentType, int compFuncEvent) const = 0;
};

class MediatorComponent {
protected:
	Mediator* _mediator;

public:

	MediatorComponent(Mediator* mediator = nullptr) : _mediator(mediator) {}

	void setMediator(Mediator* mediator) {
		this->_mediator = mediator;
	}

};

#endif // !MEDIATOR





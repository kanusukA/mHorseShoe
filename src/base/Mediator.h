#pragma once

#ifndef MEDIATOR_H
#define MEDIATOR_H

//Local
#include <GDHandler/Connector/MasterB.h>
#include <Gui/GuiComponents/ToastComponent.h>

//Third

//STL
#include <string>






// CODE CLEAN UP SHIFT TO NEW GUI FRAMEWORK!!

// GuiFRAMEWORK IMPLEMENTATION
// HERE ARE THE CLASS USED TO CREATE VIEW AND MODEL COMPONENTS FOR THE GUI

class GuiFramework;
class GDSource;

// ViewComponent is used to create elements of UI, typically a window of ImGui
// name/id are used to access the derived class from the main Gui class
// use the view function to write renderable ImGui Functions
class ViewComponent {
private:
	ViewComponent();

protected:

	GuiFramework* guiFramework;

	bool show = true;
	int id = 0;
	const char* name = "";
	// Set type

public:

	ViewComponent(const char* name_p) {
		name = name_p;
	}

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
private:
	ModelComponent();

protected:
	GuiFramework* guiFramework;
	GDSource* gdSource;

	const char* name;

public:

	ModelComponent(const char* name_p) {
		name = name_p;
	}

	void setFramework(GuiFramework* framework) {
		guiFramework = framework;
	}
	void setSource(GDSource* source) {
		gdSource = source;
	}

	// USED TO INITALIZE PREDEFINED DATA AND VARIABLE OF THE COMPONENT
	// USE THIS INSTED OF CONSTRUCTOR AS GDSOURCE IS INITALIZED AFTER THE OBJECT IS CREATED
	virtual void init() {};

	// Run when a ResourceUpdates!
	virtual void update() {};

};



// GDSource - IT IS THE CONNECTOR CLASS FOR THE GUI FRAMEWORK.
// IT PROVIDES ACCESS TO OTHER PARTS OF THE SOFTWARE/
// THIS CLASS SHOULD NOT BE DIRCETLY CONNECTED WITH THE VIEW CLASS AS IT PROVIDES LIMITLESS ACCESS OF OTHER CLASSES
// TYPICALLY MODELCOMPONENT MUST BE USED IN BETWEEN TO FILTER AND MASK DATA.
class GDSource : public GuiRegen {

private:
	GDSource() {};

	
protected:
	CaseHandler* scnHandler = nullptr;
	StuffHandler* stuffHandler = nullptr;
	ResourceHandler* resourceHandler = nullptr;
	RSUS* shaderHandler = nullptr;

	//PlayerObserver* playerObserver = nullptr;

public:
	GDSource(CaseHandler* casehan, StuffHandler* stuffhan, 
		ResourceHandler* resourceHan,
		RSUS* rsus) {
		this->scnHandler = casehan;
		this->stuffHandler = stuffhan;
		this->resourceHandler = resourceHan;
		//this->feel = feelhan;
		this->shaderHandler = rsus;
	};

	/*void setPlayerObserver(PlayerObserver* playerOb) {
		playerObserver = playerOb;
	}*/

	// TODO Add player Observer directly to GUI system

	CaseHandler* getCaseHandler() {
		return scnHandler;
	}

	StuffHandler* getStuffHandler() {
		return stuffHandler;
	}

	ResourceHandler* getResourceHandler() {
		return resourceHandler;
	}

	/*PlayerObserver* getPlayerObserver() {
		return playerObserver;
	}*/

	RSUS* getShaderHandler() {
		return shaderHandler;
	}

};


class GuiFramework : public GDSource {
	
protected:

	std::vector<ModelComponent*> Models = std::vector<ModelComponent*>();
	std::vector<ViewComponent*> Views = std::vector<ViewComponent*>();

	ModelComponent* getModelByName(std::string name) {

	};
	ModelComponent* getModelById(int id) {

	};

	void regenScenes() override{
		for (int i = 0; i < Models.size(); i++)
		{
			Models.at(i)->update();
		}
	}

public:

	GuiFramework(CaseHandler* casehan, StuffHandler* stuffhan, ResourceHandler* resourcehan, RSUS* rsus) : 
		GDSource(casehan, stuffhan,resourcehan,rsus) {
		resourcehan->setGuiRegen(this);
	}

	// GUI FUNCTIONS
	
	// CONVERTING OGRE::VECTORS TO FLOAT* 
	float* convertOgreVec2(Ogre::Vector2 vec2_p) {
		float float2[2] = {vec2_p[0],vec2_p[1]};
		return float2;
	}
	float* convertOgreVec3(Ogre::Vector3 vec3_p) {
		float float3[3] = { vec3_p[0],vec3_p[1],vec3_p[2]};
		return float3;
	}
	float* convertOgreVec4(Ogre::Vector4 vec4_p) {
		float float4[4] = { vec4_p[0],vec4_p[1],vec4_p[2],vec4_p[3]};
		return float4;
	}
	// SET OGRE::VECTOR VALUE TO FLOAT*
	void setOgreVec2(float* float2,Ogre::Vector2 vec2_p) {
		float2[0] = vec2_p[0];
		float2[1] = vec2_p[1];
	}
	void setOgreVec3(float* float3,Ogre::Vector3 vec3_p) {
		float3[0] = vec3_p[0];
		float3[1] = vec3_p[1];
		float3[2] = vec3_p[2];
	}
	void setOgreVec4(float* float4,Ogre::Vector4 vec4_p) {
		float4[0] = vec4_p[0];
		float4[1] = vec4_p[1];
		float4[2] = vec4_p[2];
		float4[3] = vec4_p[3];
	}

	// RUNS AT INIT, USED DURING THE CREATION OF VIEW COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// VIEW COMPONENTS MUST BE ADDED TO BE RENDERED!
	virtual void addViewComponent(ViewComponent* viewComponent) {};

	// RUNS AT INIT, USED DURING THE CREATION OF MODEL COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// MODEL COMPONENTS MUST BE ADDED TO BE RENDERED
	virtual void addModelComponent(ModelComponent* modelComponent) {};
		

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





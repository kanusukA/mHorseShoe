#pragma once

#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <string>
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





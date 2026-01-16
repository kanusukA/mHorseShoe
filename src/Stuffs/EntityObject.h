#pragma once

#include <Stuffs/RenderMesh.h>


class Object : public ObjectResource {
public:
	GDBuilderContext* builderCxt;

	std::unique_ptr<Ogre::Entity> entity;

	std::shared_ptr<Material> material;

	Object(GDBuilderContext* builderCxt_p, Ogre::Entity* entity_p, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), name_p, objType_p) {
		builderCxt = builderCxt_p;
		entity.reset(entity_p);

	}

	// TO set Material to an Object use this method. Assigning material from RenderMesh will not work!
	void setMaterial(std::string materialPath_p) {
		//builderCxt->addMaterial(materialPath_p);
	}




};



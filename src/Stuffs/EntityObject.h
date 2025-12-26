#pragma once

#include <GDHandler/GDContext.h>

class Object : public ObjectResource {
private:
	GDBuilderContext* builderCxt;

	Ogre::Entity* entity;
	PxRigidDynamic* rigidDyn;
	PxRigidStatic* rigidStatic;

public:

	Object(GDBuilderContext* builderCxt_p, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), name_p, objType_p) {
		builderCxt = builderCxt_p;
		entity = builderCxt->createObject(name_p);
		
	}

	void setPxRigidBody() {

	}

	


};
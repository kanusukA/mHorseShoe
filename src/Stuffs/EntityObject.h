#pragma once

#include <GDHandler/GDContext.h>

class Object : public ObjectResource {
private:
	GDBuilderContext* builderCxt;

	Ogre::Entity* entity;
	/*PxRigidDynamic* rigidDyn = nullptr;
	PxRigidStatic* rigidStatic = nullptr;*/

public:

	Object(GDBuilderContext* builderCxt_p,ResID renderMeshID, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(),this, name_p, objType_p) {
		builderCxt = builderCxt_p;

		//RenderMesh* renderMesh = ResourceHandler::GetInstance()->fetchRenderMeshResourceByID(renderMeshID).get;
		//entity = builderCxt->createObject(name_p,);
		
	}

	void setPxRigidBody() {
		// TODO set object physx
	}

	


};
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

		entity = builderCxt->createObject(name_p, ResourceHandler::GetInstance()->fetchRenderMeshResourceByID(renderMeshID)->getName());
		
	}

	Object(GDBuilderContext* builderCxt_p, RenderMeshResource* renderMesh, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), this, name_p, objType_p) {
		builderCxt = builderCxt_p;

		entity = builderCxt->createObject(name_p, renderMesh->getName());

	}

	void setPxRigidBody() {
		// TODO set object physx
	}

	~Object() {

		if (entity)
		{
			entity->detachFromParent();
			delete entity;
		}
	}

	


};
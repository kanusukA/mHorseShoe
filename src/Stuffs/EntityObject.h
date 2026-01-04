#pragma once

#include <Stuffs/RenderMesh.h>


class Object : public ObjectResource {
private:
	GDBuilderContext* builderCxt;

	Ogre::Entity* entity;

	RenderMesh* mesh;
	ColliderMesh* cMesh;

public:

	Object(GDBuilderContext* builderCxt_p,ResID renderMeshID, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(),this, name_p, objType_p) {
		builderCxt = builderCxt_p;
		mesh = ResourceHandler::GetInstance()->fetchRenderMeshResourceByID(renderMeshID)->getHigherClass();
		entity = builderCxt->createObject(name_p, mesh->getName());
		
	}

	Object(GDBuilderContext* builderCxt_p, RenderMeshResource* renderMesh, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), this, name_p, objType_p) {
		builderCxt = builderCxt_p;
		mesh = renderMesh->getHigherClass();
		entity = builderCxt->createObject(name_p, renderMesh->getName());

	}

	void setPxRigidBody() {

		// TODO set object physx

	}

	RenderMesh* getRenderMesh() {
		return mesh;
	}

	ColliderMesh* getColliderMesh() {
		return cMesh;
	}

	Ogre::Entity* getEntity() {
		return entity;
	}

	/*
	void setRenderMesh(RenderMesh* mesh_p) {
		_setRenderMesh(mesh_p->getId());
		mesh = mesh_p;

	}
	void setColliderMesh(ColliderMesh* mesh_p) {
		_setColliderMesh(mesh_p->getId());
		cMesh = mesh_p;
	}
	*/

	~Object() {

		if (entity)
		{
			entity->detachFromParent();
			delete entity;
		}
	}

	


};

class ObjectPtr {
private:
	Object* object;

public:
	
	Object* get()
	{
		return object;
	}

	void set(Object* obj) {
		object = obj;
	}


};
#pragma once

#include <Stuffs/RenderMesh.h>


class Object : public ObjectResource {
private:
	GDBuilderContext* builderCxt;

	Ogre::Entity* entity;

	ColliderMesh* cMesh;

	Material* material;

public:

	/*Object(GDBuilderContext* builderCxt_p,ResID renderMeshID, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(),this, name_p, objType_p) {
		builderCxt = builderCxt_p;
		entity = builderCxt->createObject(name_p, ResourceHandler::GetInstance()->);

	}*/

	Object(GDBuilderContext* builderCxt_p, RenderMesh* renderMesh, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), this, name_p, objType_p) {
		builderCxt = builderCxt_p;
		entity = builderCxt->createObject(name_p, renderMesh->getMesh());

	}

	// TO set Material to an Object use this method. Assigning material from RenderMesh will not work!
	void setMaterial(Material* mat_p) {
		material = mat_p;
		entity->setMaterial(mat_p->getMaterialPtr());
	}

	// use this method to manage Object related Material
	Material* getMaterial() {
		return material;
	}

	Ogre::MeshPtr getMesh() {
		return entity->getMesh();
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
#pragma once

#include <Stuffs/ColliderMesh.h>

class RenderMesh : public RenderMeshResource {

private:

	Ogre::MeshPtr mesh;
	GDBuilderContext* builderCxt;


public:

	// Material must be added separately
	RenderMesh(GDBuilderContext* builderCxt_p,Ogre::MeshPtr mesh_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(), mesh_p->getName() ) {
		builderCxt = builderCxt_p;


	}

	Ogre::MeshPtr getMesh() {
		return mesh;
	}

};
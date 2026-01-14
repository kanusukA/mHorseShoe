#pragma once

#include <Stuffs/ColliderMesh.h>

class RenderMesh : public RenderMeshResource {

private:

	Ogre::MeshPtr mesh;
	GDBuilderContext* builderCxt;


public:

	// Material must be added separately
	RenderMesh(GDBuilderContext* builderCxt_p,std::string meshName_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(),this, meshName_p ) {
		builderCxt = builderCxt_p;
		mesh = builderCxt->createMesh(meshName_p);


	}

	Ogre::MeshPtr getMesh() {
		return mesh;
	}

};
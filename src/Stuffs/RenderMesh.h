#pragma once

#include <Stuffs/ColliderMesh.h>

class RenderMesh : public RenderMeshResource {

private:

	Ogre::MeshPtr mesh;
	GDBuilderContext* builderCxt;


public:

	RenderMesh(GDBuilderContext* builderCxt_p,Ogre::MeshPtr mesh_p, std::filesystem::path meshPath_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(), mesh_p->getName(), meshPath_p ) {
		builderCxt = builderCxt_p;

	}

	Ogre::MeshPtr getMesh() {
		return mesh;
	}

	~RenderMesh() {
		// MESHES ARE DELETED BY THE RESOURCE AS UNLOADING AND RELOADING IS WASTEFUL. IF A MESH IS NO LONGER NEEDED, IT SHOULD BE DELETED MANUALLY.
	}

};
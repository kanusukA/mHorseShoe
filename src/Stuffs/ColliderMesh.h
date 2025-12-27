#pragma once

#include <GDHandler/GDContext.h>

class ColliderMesh : public ColliderMeshResource {

private:

	Ogre::Mesh* mesh;
	GDBuilderContext* builderCxt;

public:

	// Material must be added separately
	ColliderMesh(GDBuilderContext* builderCxt_p, std::string meshName_p) :
		ColliderMeshResource(ResourceHandler::GetInstance(), meshName_p) {
		builderCxt = builderCxt_p;
		mesh = builderCxt->createMesh(meshName_p);

	}

	void setMaterial(ResID matID, Ogre::MaterialPtr mat_p) {
		mesh->getSubMesh(0)->setMaterial(mat_p);
		material = matID;
	}

};
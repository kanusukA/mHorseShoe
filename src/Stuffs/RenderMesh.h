#pragma once

#include <GDHandler/GDContext.h>

class RenderMesh : public RenderMeshResource {

private:

	Ogre::Mesh* mesh;
	GDBuilderContext* builderCxt;

public:

	// Material must be added separately
	RenderMesh(GDBuilderContext* builderCxt_p,std::string meshName_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(),this, meshName_p ) {
		builderCxt = builderCxt_p;
		mesh = builderCxt->createMesh(meshName_p);

	}

	void setMaterial(ResID matID, Ogre::MaterialPtr mat_p) {
		mesh->getSubMesh(0)->setMaterial(mat_p);
		material = matID;
	}

};
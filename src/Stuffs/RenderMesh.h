#pragma once

#include <Stuffs/ColliderMesh.h>

class RenderMesh : public RenderMeshResource {

private:

	Ogre::MeshPtr mesh;
	GDBuilderContext* builderCxt;

	Material* material;

public:

	// Material must be added separately
	RenderMesh(GDBuilderContext* builderCxt_p,std::string meshName_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(),this, meshName_p ) {
		builderCxt = builderCxt_p;
		mesh = builderCxt->createMesh(meshName_p);


	}

	void setMaterial(Material* mat_p) {
		mesh.get()->getSubMesh(0)->setMaterial(mat_p->getMaterialPtr());
		materialID = mat_p->getId();
		material = mat_p;
	}

	Material* getMaterial() {
		return material;
	}

	// Used to check the actual assigned mesh material
	std::string getMeshMaterialName() {
		return mesh.get()->getSubMesh(0)->getMaterialName();
	}

	Ogre::MeshPtr getMesh() {
		return mesh;
	}

};
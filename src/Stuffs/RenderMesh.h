#pragma once

#include <GDHandler/GDContext.h>

class RenderMesh : public RenderMeshResource {
private:
	Ogre::Mesh* mesh;
	GDBuilderContext* builderCxt;
public:

	RenderMesh(GDBuilderContext* builderCxt_p,std::string name_p,std::string meshName_p, std::string meshFileName_p) : 
		RenderMeshResource(ResourceHandler::GetInstance(), name_p, meshName_p, meshFileName_p) {
		builderCxt = builderCxt_p;
		builderCxt->createRenderMesh(meshName_p);
		
	}

};
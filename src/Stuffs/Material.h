#pragma once

#include <GDHandler/GDContext.h>

class Material : public  MaterialResource {
private:

	Ogre::MaterialPtr material;
	GDBuilderContext* builderCxt;

public:

	Material(GDBuilderContext* builderCxt_p,std::string matName_p) : MaterialResource(ResourceHandler::GetInstance(),this, matName_p) {
		builderCxt = builderCxt_p;

		material = builderCxt->createMaterial(matName_p);
		// Make sure to set Shaders when creating Material.


	}

	Ogre::MaterialPtr getMaterialPtr() {
		return material;
	}


};
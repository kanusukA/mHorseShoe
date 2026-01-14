#pragma once

#include <ResourceHandler/ResourceObjects.h>



class Material : public  MaterialResource {
private:

	Ogre::MaterialPtr material;

	Shader* vertexShader;
	Shader* fragmentShader;


	Material();

public:

	//Material(GDBuilderContext* builderCxt_p,std::string matName_p) : MaterialResource(ResourceHandler::GetInstance(),this, matName_p) {
	//	builderCxt = builderCxt_p;

	//	material = builderCxt->createMaterial(matName_p);
	//	// Make sure to set Shaders when creating Material.


	//}

	void setVertexShader(Shader* vertex_p) {
		MaterialResource::_addVertexShader(vertex_p->getId());
		vertexShader = vertex_p;
	}
	void setFragmentShader(Shader* fragment_p) {
		MaterialResource::_addFragmentShader(fragment_p->getId());
		fragmentShader = fragment_p;
	}

	Shader* getFragmentShader() {
		return fragmentShader;
	}
	Shader* getVertexShader() {
		return vertexShader;
	}

	Ogre::MaterialPtr getMaterialPtr() {
		return material;
	}


};

// This class helps in pointer management and apt resource creation and assignment of resources

// Material Resource in horseshoee is consists of a single primary reference, which is a unique_ptr owned by the ResourceBuilderCxt.
// This reference can be used to create more copies of the material or shared material.
// It is importent to note that primary Material is created by ResourceBuilderCxt and this class only manages the distribution of that Material.
class MaterialManager {

public:
	ResID createMaterial()

};
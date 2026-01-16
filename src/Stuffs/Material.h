#pragma once

#include <Stuffs/ShaderObject.h>

class Material : public  MaterialResource {
protected:

	Ogre::MaterialPtr material;

	// TODO convert to shared_ptr
	std::unique_ptr<Shader> vertexShader;
	std::unique_ptr<Shader> fragmentShader;

	


public:

	Material(GDBuilderContext* builderCxt, Ogre::MaterialPtr material_p ) : MaterialResource(ResourceHandler::GetInstance(),material_p->getName()) {
		material = material_p;
	};

	

	//void setVertexShader(Shader* vertex_p) {
	//	/*MaterialResource::_addVertexShader(vertex_p->getId());
	//	vertexShader = vertex_p;*/
	//}
	//void setFragmentShader(Shader* fragment_p) {
	//	/*MaterialResource::_addFragmentShader(fragment_p->getId());
	//	fragmentShader = fragment_p;*/
	//}

	

	Ogre::MaterialPtr getMaterialPtr() {
		return material;
	}


};

// This class helps in pointer management and apt resource creation and assignment of resources

// Material Resource in horseshoee is consists of a single primary reference, which is a unique_ptr owned by the ResourceBuilderCxt.
// This reference can be used to create more copies of the material or shared material.
// It is important to note that primary Material is created by ResourceBuilderCxt and this class only manages the distribution of that Material.
class MaterialManager  {
private:
	ResourceHandler* resourceHan = ResourceHandler::GetInstance();
protected:
	
	MaterialManager() {
		
	};

public:

	ResID createMaterialResource(std::string mat_name);

	// A new instance of called material is created. The returned unique_ptr is attached to the Owner Object.
	Material* fetchNewMaterial(ResID materialID, Ogre::MaterialPtr matPtr);

};
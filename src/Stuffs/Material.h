#pragma once

#include <Stuffs/ShaderObject.h>

class Material : public  MaterialResource {
protected:

	Ogre::MaterialPtr material;

	// TODO convert to shared_ptr
	Shader* vertexShader;
	Shader* fragmentShader;

	Material() {};


public:

	void initMaterial(Ogre::MaterialPtr mat_p) {
		material = mat_p;
	}

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

	Material(const MaterialResource& materialObj) {
		materialName = materialObj.materialName;
		setId(materialObj._id);
		VertexShaderResource = materialObj.VertexShaderResource;
		FragmentShaderResource = materialObj.FragmentShaderResource;
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
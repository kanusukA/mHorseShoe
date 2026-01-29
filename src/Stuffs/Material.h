#pragma once

#include <Stuffs/ShaderObject.h>

// TODO DELETE MATERIAL_PTR ??

class Material : public  MaterialResource {
protected:
	GDBuilderContext* GDBuilderCxt;

	Ogre::MaterialPtr material;

	
	std::unique_ptr<Shader> vertexShader;
	std::unique_ptr<Shader> fragmentShader;

	Ogre::CullingMode culling = Ogre::CULL_CLOCKWISE;

	bool wireFrameMode = false;
	
public:

	Material(GDBuilderContext* GDBuilderCxt_p, Ogre::MaterialPtr material_p, std::string materialFilePath ) :
		MaterialResource(ResourceHandler::GetInstance(),material_p->getName(), materialFilePath) {
		material = material_p;
		GDBuilderCxt = GDBuilderCxt_p;
		setVertexShader();
		setFragmentShader();

		if (vertexShader && fragmentShader)
		{
			GDBuilderCxt->monProvideRsus()->setShader(material->getName(),
				fragmentShader->getName(), vertexShader->getName(),
				fragmentShader->getShaderVars(), vertexShader->getShaderVars(),
				fragmentShader->getShader(), vertexShader->getShader());
		}
		else {
			ToastComponent::GetInstance()->addMessage("Failed to create Material Shaders, for Material : " + material->getName());
		}

	};

	void setVertexShader() {
		vertexShader = std::make_unique<Shader>(GDBuilderCxt, material, ShaderType::Vertex);
	}

	void setFragmentShader() {
		fragmentShader = std::make_unique<Shader>(GDBuilderCxt, material, ShaderType::Fragment);
	}

	const std::unique_ptr<Shader>& const getVertexShader() {
		return vertexShader;
	}

	const std::unique_ptr<Shader>& const getFragmentShader() {
		return fragmentShader;
	}

	void setCullingMode(Ogre::CullingMode culling_p);
	const Ogre::CullingMode& const getCullingMode() {
		return culling;
	}
	void setWireFrameMode(bool mode_p);
	const bool& const getWireFrameMode() {
		return wireFrameMode;
	}


	

	Ogre::MaterialPtr getMaterialPtr() {
		return material;
	}


};

// This class helps in pointer management and apt resource creation and assignment of resources

// Material Resource in horseshoee is consists of a single primary reference, which is a unique_ptr owned by the ResourceBuilderCxt.
// This reference can be used to create more copies of the material or shared material.
// It is important to note that primary Material is created by ResourceBuilderCxt and this class only manages the distribution of that Material.
//class MaterialManager  {
//private:
//	ResourceHandler* resourceHan = ResourceHandler::GetInstance();
//protected:
//	
//	MaterialManager() {
//		
//	};
//
//public:
//
//	ResID createMaterialResource(std::string mat_name);
//
//	// A new instance of called material is created. The returned unique_ptr is attached to the Owner Object.
//	Material* fetchNewMaterial(ResID materialID, Ogre::MaterialPtr matPtr);
//
//};
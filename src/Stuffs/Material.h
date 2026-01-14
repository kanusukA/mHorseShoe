#pragma once

#include <Stuffs/ShaderObject.h>

//class Material : public  MaterialResource {
//private:
//
//	Ogre::MaterialPtr material;
//
//	Shader* vertexShader;
//	Shader* fragmentShader;
//
//
//	Material();
//
//public:
//
//	//Material(GDBuilderContext* builderCxt_p,std::string matName_p) : MaterialResource(ResourceHandler::GetInstance(),this, matName_p) {
//	//	builderCxt = builderCxt_p;
//
//	//	material = builderCxt->createMaterial(matName_p);
//	//	// Make sure to set Shaders when creating Material.
//
//
//	//}
//
//	void setVertexShader(Shader* vertex_p) {
//		MaterialResource::_addVertexShader(vertex_p->getId());
//		vertexShader = vertex_p;
//	}
//	void setFragmentShader(Shader* fragment_p) {
//		MaterialResource::_addFragmentShader(fragment_p->getId());
//		fragmentShader = fragment_p;
//	}
//
//	Shader* getFragmentShader() {
//		return fragmentShader;
//	}
//	Shader* getVertexShader() {
//		return vertexShader;
//	}
//
//	Ogre::MaterialPtr getMaterialPtr() {
//		return material;
//	}
//
//
//};
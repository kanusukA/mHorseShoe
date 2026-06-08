#pragma once

#include <Stuffs/RenderMesh.h>

// clean each object file with new object creation standard!
// use default destructors or change a newer implementation if modern C++ has something as such!


class Object : public ObjectResource {

protected:

	
public:

	GDBuilderContext* GDBuilderCxt;

	std::shared_ptr<Ogre::Entity> entity;

	std::shared_ptr<Material> sMaterial;

	Object(GDBuilderContext* GDBuilderCxt_p, Ogre::Entity* entity_p, std::string name_p, PhysXType objType_p,std::filesystem::path meshPath_p) : 
		ObjectResource(ResourceHandler::GetInstance(), name_p, objType_p,meshPath_p) {
		GDBuilderCxt = GDBuilderCxt_p;

		this->renderMeshName = entity_p->getName();
		
		entity.reset(entity_p);

		if (entity.get()->getMesh().get()->getSubMeshes().size() > 0 && entity.get()->getMesh().get()->getSubMesh(0)->getMaterial())
		{
			Material* newMat = new Material(GDBuilderCxt_p, entity.get()->getMesh().get()->getSubMesh(0)->getMaterial(), "");
			sMaterial.reset(newMat);
		}
		
	}

	// materialName must differ from materialPath_p. Else exception will be created!
	bool setMaterial(std::filesystem::path materialPath_p, std::string materialName_p) {
		Material* newMat = GDBuilderCxt->CreateMaterial(materialPath_p, materialName_p);
		if (newMat)
		{
			sMaterial.reset(newMat);
			entity->setMaterial(sMaterial->getMaterialPtr());

			return true;
		}
		return false;
		
	}

	const std::weak_ptr<Material> getwMaterial() {
		return sMaterial;
	}

	const std::string getMeshName() {
		return entity->getMesh().get()->getName();
	}

	const std::string getMeshMaterialName() {
		return entity->getMesh().get()->getSubMesh(0)->getMaterialName();
	}

	// DELETOR
	~Object() {
		destroyObject();
	}

	void destroyObject() {
		GDBuilderCxt->monDeleteEntity(entity.get());
	}

};


